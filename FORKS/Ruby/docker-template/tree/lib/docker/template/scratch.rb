# ----------------------------------------------------------------------------
# Frozen-string-literal: true
# Copyright: 2015 - 2016 Jordon Bedwell - Apache v2.0 License
# Encoding: utf-8
# ----------------------------------------------------------------------------

module Docker
  module Template
    class Scratch < Builder
      attr_reader :rootfs

      # ----------------------------------------------------------------------

      def initialize(*args)
        super; @rootfs = Rootfs.new(
          repo
        )
      end

      # ----------------------------------------------------------------------

      def data
        Template.get(:scratch, {
          :entrypoint => @repo.metadata.entry,
          :maintainer => @repo.metadata.maintainer,
          :tar_gz => @tar_gz.basename
        })
      end

      # ----------------------------------------------------------------------

      def teardown(img: false)
        @copy.rm_rf if @copy
        @context.rm_rf if @context
        @tar_gz.rm_rf if @tar_gz

        if @img && img
          then @img.delete({
            "force" => true
          })
        end
      rescue Docker::Error::NotFoundError
        nil
      end

      # ----------------------------------------------------------------------

      private
      def setup_context
        @context = @repo.tmpdir
        @tar_gz = @repo.tmpfile "archive", ".tar.gz", root: @context
        @copy = @repo.tmpdir "copy"
        copy_dockerfile
      end

      # ----------------------------------------------------------------------

      private
      def copy_dockerfile
        data = self.data % @tar_gz.basename
        dockerfile = @context.join("Dockerfile")
        dockerfile.write(data)
      end

      # ----------------------------------------------------------------------

      def copy_cleanup
        @rootfs.simple_cleanup(
          @copy
        )
      end

      # ----------------------------------------------------------------------

      def verify_context
        if @repo.buildable? && @tar_gz.zero?
          raise Error::InvalidTargzFile, @tar_gz
        end
      end

      # ----------------------------------------------------------------------

      private
      def build_context
        return unless @repo.buildable?
        @rootfs.build

        img = Container.create(create_args)
        img.start(start_args).attach(logger_opts, &Logger.new.method(logger_type))
        status = img.json["State"]["ExitCode"]

        if status != 0
          raise Error::BadExitStatus, status
        end
      ensure
        @rootfs.teardown

        if img
          then img.tap(&:stop).delete({
            "force" => true
          })
        end
      end

      # ----------------------------------------------------------------------

      private
      def logger_type
        @repo.metadata["tty"] ? :tty : :simple
      end

      # ----------------------------------------------------------------------

      private
      def logger_opts
        return {
          :tty => @repo.metadata["tty"], :stdout => true, :stderr => true
        }
      end

      # ----------------------------------------------------------------------

      private
      def create_args
        name = ["rootfs", @repo.name, @repo.tag, "image"].join("-")
        env  = @repo.to_env(:tar_gz => @tar_gz, :copy_dir => @copy)

        return {
          "Env"     => env.to_a,
          "Tty"     => @repo.metadata["tty"],
          "Image"   => @rootfs.img.id,
          "Name"    => name,

          "HostConfig" => {
            "Binds" => [
              "#{@copy}:#{@copy}", "#{@tar_gz}:#{@tar_gz}"
            ]
          },

          "Volumes" => {
            @copy.to_s   => {
              "source" => @copy.to_s,
              "destination" => @copy.to_s
            },

            @tar_gz.to_s => {
              "source" => @tar_gz.to_s,
              "destination" => @tar_gz.to_s
            }
          }
        }
      end

      # ----------------------------------------------------------------------

      private
      def start_args
        {
          # "Binds" => [
          #   "#{@copy}:#{@copy}", "#{@tar_gz}:#{@tar_gz}"
          # ]
        }
      end
    end
  end
end
