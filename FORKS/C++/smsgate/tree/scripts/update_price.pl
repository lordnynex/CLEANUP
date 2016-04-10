#!/usr/bin/perl

use DBI;

$dbh = DBI->connect("dbi:Pg:dbname=smsgate","biin","a6sent",
		    {PrintError => 0});

if ($DBI::err != 0) {
  print $DBI::errstr . "\n";
  exit($DBI::err);
}

$query = 'SELECT message_status."REQUESTID", "MESSAGEID", tariff, "COUNTRY", "OPERATORCODE" FROM '.
	 'message_status, smsrequest, partners where '.
	 'message_status."REQUESTID"=smsrequest."REQUESTID" and smsrequest."PID"=partners.pid::text '.
	 'and smsrequest."PID"=\'103\' and smsrequest."WHEN">=ts2int(TIMESTAMP \'2011-02-01\');';

$sth = $dbh->prepare($query);
$rv = $sth->execute();
if (!defined $rv) {
  print "При выполнении запроса '$query' возникла ошибка: " . $dbh->errstr . "\n";
  exit(0);
}



$line = 0;
while (@array = $sth->fetchrow_array()) {
  ($reqid, $msgid, $tariff, $country, $operator) = @array;
  $line++;

  $price = `./thelper tariff_check $tariff $country $operator`; $price =~ s/\n//;
  print "[$line]\t$tariff: $country [$operator]=$price\n";
  $update_query = 'update message_status set "PARTNERPRICE"='.$price.' where "REQUESTID"='.$reqid.' and "MESSAGEID"='.$msgid.';';
  #$update_sth = $dbh->prepare( $update_query );
  #$update_rv = $update_sth->execute();

  if (!defined $update_rv) {
    print "При выполнении запроса '$update_query' возникла ошибка: " . $dbh->errstr . "\n";
    exit(0);
  }
}

$sth->finish();
$dbh->disconnect();
