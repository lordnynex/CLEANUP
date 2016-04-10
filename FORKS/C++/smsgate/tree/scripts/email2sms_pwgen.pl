use strict;
use warnings;
use Digest::MD5 qw(md5_hex);
use URI::Escape qw( uri_escape_utf8 );

binmode STDOUT, ":utf8";
my $salt = "Our love will never die";

my $key = gen_id( $ARGV[0], $ARGV[1], $salt );
printf( "%s\@greensms.ru\n", $key );

sub gen_id {
	my $idp = shift;
	my $from = shift;
	my $salt = shift;	

	my $hash = substr( (md5_hex $idp.$from.$salt), 0, 6 );
	return $from.$hash.$idp;
}
