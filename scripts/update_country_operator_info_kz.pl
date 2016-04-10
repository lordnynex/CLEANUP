#!/usr/bin/perl -w
use strict;
use DBI;

my $dbh = DBI->connect("dbi:Pg:dbname=smsgate","biin","a6sent",
		    {PrintError => 0});

if ($DBI::err != 0) {
  print $DBI::errstr . "\n";
  exit($DBI::err);
}

my $query = 	'SELECT message_status."REQUESTID", "MESSAGEID", tariff, message_status."TO", "STATUS" FROM '.
         	'message_status, smsrequest, partners where '.
	 	'message_status."REQUESTID"=smsrequest."REQUESTID" and smsrequest."PID"=partners.pid::text '.
		'and message_status."COUNTRY"=\'Russian Federation\' '.
	 	'and smsrequest."PID"=\'103\' and smsrequest."WHEN">ts2int(TIMESTAMP \'2011-03-01\');';

my $sth = $dbh->prepare($query);
my $rv = $sth->execute();
if (!defined $rv) {
  print "При выполнении запроса '$query' возникла ошибка: " . $dbh->errstr . "\n";
  exit(0);
}

open( TARIFF_HELPER, "| ./thelper_v2 check > tout.log" );

my @data;
while ( my @array = $sth->fetchrow_array() ) {
  (my $reqid, my $msgid, my $tariff, my $to, my $status) = @array;

  if ( $tariff eq "" ) { $tariff="null"; }
  printf TARIFF_HELPER "$tariff $to $status\n";
  push @data, \@array;
}

close( TARIFF_HELPER );

open( TARIFF_OUT, "tout.log" );

while ( <TARIFF_OUT> ) {
  my $logline = $_;
  my $data_ref = $data[ 0 ];
  shift @data;

  (my $country_name, my $country_code, my $operator_name, my $operator_code, my $region, my $price)= split( ';', $logline);

  ( my $reqid, my $msgid, my $tariff, my $to, my $status) = @{$data_ref};
  my $update_query = "update message_status set \"COUNTRY\"='".$country_name.
                     "', \"COUNTRYCODE\"='".$country_code.
                     "', \"OPERATOR\"='".$operator_name.
                     "', \"OPERATORCODE\"='".$operator_code.
                     "', \"REGION\"='".$region.
                     "' where \"REQUESTID\"=".$reqid." and \"MESSAGEID\"=".$msgid.";";

  printf "$update_query\n";
}

$sth->finish();
$dbh->disconnect();
