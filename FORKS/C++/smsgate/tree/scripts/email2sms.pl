use strict;
use warnings;
use Net::IMAP::Simple;
use Email::MIME;
use LWP::UserAgent;
use HTTP::Request; 
use Digest::MD5  qw(md5_hex);
use URI::Escape qw( uri_escape_utf8 );

binmode STDOUT, ":utf8";
my $salt = "Our love will never die";

#printf( "%s\n", gen_id( "104", $salt ) );

# Create the object
my $imap = Net::IMAP::Simple->new('imap.yandex.ru', use_ssl=>1) ||
die "Unable to connect to IMAP: $Net::IMAP::Simple::errstr\n";

# Log on
if(!$imap->login('email2sms@greensms.ru','a6sent')){
   print STDERR "Login failed: " . $imap->errstr . "\n";
   exit(64);
}

my $nm = $imap->select('INBOX') || 
die "Error while selecting from INBOX: ".$imap->errstr."\n";

for(my $i = 1; $i <= $nm; $i++){
    	my $email = Email::MIME->new( join '', @{ $imap->get($i) } );
	my $from = $email->header('From');
	my $to = $email->header('To');
	my $subj = $email->header('Subject');
	my $body = "";

	my @parts = $email->parts; # These will be Email::MIME objects, too.
	foreach my $part ( @parts ) {
		if ( $part->content_type =~ "text/plain" ) {
			$body = $part->body_str;
		}
	}

	$body =~ s/\n$//;
	my $key = (split '@', $to)[0];
	my $key_info = check_id( $key, $salt );

   	printf ( "Found email to %s: ", $to );
	if ( $key_info->{'valid'} ) {
		my $idp = $key_info->{'idp'};
		my $from = $key_info->{'from'};

		printf( "IDP=%s; ", $idp );
		
		if ( !$body ) {
			printf( "Message is empty, nothing to send; removing message" );
			$imap->delete( $i );
		}

		if ( !$idp ) {
			printf( "IDP is empty, nothing to send; removing message" );
			$imap->delete( $i );
		}

		if ( !$subj ) {
			printf( "TO is empty, nothing to send; removing message" );
			$imap->delete( $i );
		}
																	
		printf( "FROM=%s; TO=%s; TXT=\"%s\" ", $from, $subj, uri_escape_utf8($body) );
		my $sent = sms_notify( $idp, $from, $subj, $body );
		if ( $sent ) {
			printf( "SMS was successfully sent; removing message" );
			$imap->delete( $i );
		} else {
			printf( "Error while sending sms" );
			$imap->delete( $i );
		}
	} else {
		printf( "Invalid recepient; removing message" );
			$imap->delete( $i );
	}
	print "\n";
	
}

$imap->quit;

sub sms_notify {
        my $idp = shift;
        my $from = shift;
	my $to = shift;
        my $nreport = uri_escape_utf8(shift);

        my $ua = LWP::UserAgent->new;
        my $req = HTTP::Request->new(POST=>"http://sender.greensms.ru/mt.cgi");
        $req->content_type('application/x-www-form-urlencoded');
        $req->content("idp=$idp&dlr=1&utf=1&from=$from&txt=$nreport&user=god&pass=3dfx15gh&to=$to");
        my $res = $ua->request($req)->content();
        return $res =~ "<code>0</code>";
}

sub check_id {
	my $key = shift;
	my $salt = shift;	

	my %info;
	$info{'valid'} = 0;

	if ( !$key ) {
		return \%info;	
	}

	if ( !$salt ) {
		return \%info;	
	}

	$key =~ /^(.*)([0-9a-fA-F]{6})([0-9]{3})$/;
	my $from=$1;
	my $hash=$2;
	my $idp=$3;

	$info{'idp'}=$idp;
	$info{'from'}=$from;

	#printf("FROM=%s HASH=%s IDP=%s\n", $from, $hash, $idp );

	if ( gen_id( $idp, $from, $salt ) eq $key ) {
		$info{'valid'} = 1;
		return \%info;
	}
	return \%info;
}

sub gen_id {
	my $idp = shift;
	my $from = shift;
	my $salt = shift;	

	my $hash = substr( (md5_hex $idp.$from.$salt), 0, 6 );
	return $from.$hash.$idp;
}
