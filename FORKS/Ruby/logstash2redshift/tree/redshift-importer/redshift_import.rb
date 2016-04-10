#!/usr/bin/env ruby

require 'aws-sdk'
require 'date'
require 'fileutils'
require 'logger'
require 'pg'

AWSKEY = ""
AWSSECRET = ""
IMPORT_BUCKET = "eventlogsimport"

RS_HOST = ""
RS_PORT = 5439
RS_USER = ""
RS_PASS = ""
RS_DB = ""

LOG_SOURCE_DIR = "/opt/redshift_logs/event_logger/"
EVENTS_TO_IMPORT = ["login_attempt", "pto_request"]

FileUtils::mkdir_p LOG_SOURCE_DIR

LOG_FILE = "/var/log/redshift_imports.log"
FileUtils.mkdir_p(File.dirname(LOG_FILE))
@logger = Logger.new(LOG_FILE, shift_age = 7, shift_size=1048576)

now_date = DateTime.now
current_hour = now_date.hour

@logger.info("Starting import of event logs.")


EVENTS_TO_IMPORT.each do |event_name|
  # Logstash is storing event logs for import in hourly denoted directories.
  # For each log event we're importing we cycle through all the logs
  # available except for the current hou and import them into an already created table.

  event_dir = "#{LOG_SOURCE_DIR}#{event_name}"

  file_count = 0
  Dir["#{event_dir}/*"].each do |log_file|
    begin
      @logger.info("Importing #{log_file}")

      #Skip the log file currently being written to by the aggregator
      if log_file.end_with?("_#{current_hour}.log")
        next
      end

      file_count += 1

      # Move the current file being imported into a temporary location.
      s3_filename = "#{event_name}_#{now_date.strftime("%Y%m%d-%H%M%S")}_#{file_count}.log"

      #Upload the file from it's temporary location to proper bucket in s3.
      bucket_key = "#{event_name}/#{now_date.strftime("%Y-%m-%d")}/#{s3_filename}"

      s3 = Aws::S3::Resource.new(
        credentials:Aws::Credentials.new(AWSKEY, AWSSECRET),
        region:'us-east-1')
      obj = obj = s3.bucket(IMPORT_BUCKET).object(bucket_key)
      obj.upload_file(log_file)

      #Tell redshift to import the s3 file into the event logs tables.
      query = "
        COPY
          eventlogs.#{event_name}
        FROM
          's3://#{IMPORT_BUCKET}/#{bucket_key}'
        credentials 'aws_access_key_id=#{AWSKEY};aws_secret_access_key=#{AWSSECRET}'
        json 'auto'
        DATEFORMAT as  'auto' ACCEPTANYDATE;
      "
      rs_client = PG.connect(:host=>RS_HOST, :port=>RS_PORT, :dbname=>RS_DB, :user=>RS_USER, :password=>RS_PASS)
      rs_client.exec(query)

      #Delete the successfully imported file.
      FileUtils.rm(log_file)

      @logger.info("Successful import of #{log_file}")
    rescue Exception => e
      @logger.error("Error importing #{event_name}")
      @logger.error e
    end
  end

  @logger.info("Import complete")

end

