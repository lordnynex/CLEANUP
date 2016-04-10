#!/usr/bin/perl

#       author               Daniel Dominik Rudnicki
#       thanks to:         Piotr Romanczuk
#       email                  daniel@sardzent.org
#       version              0.4.3
#       webpage           http://www.nginx.eu/
#
#       BASED @ http://wiki.codemongers.com/NginxSimpleCGI

# use strict;

use FCGI;
use Getopt::Long;
use IO::All;
use Socket;

sub init {
	GetOptions(     "h"   => \$help,
		"verbose!"=>\$verbose,
		"pid=s"     => \$filepid,
		"l=s" => \$logfile,
		"S:s"   => \$unixsocket,
		"P:i"   => \$unixport) or usage();
	usage() if $help;

	print "       Starting Nginx-fcgi\n" if $verbose;
	print "       Running with $> UID" if $verbose;
	print "       Perl $]" if $verbose;

	if ( $> == "0" ) {
		print "\n\tERROR\tRunning as a root!\n";
		print "\tSuggested not to do so !!!\n\n";
		exit 1;
	}

	if ( ! $logfile ) {
		print "\n\tERROR\t log file must declared\n"
		. "\tuse $0 with option -l filename\n\n";
		exit 1;
	}
	print "       Using log file $logfile\n" if $verbose;
	"\n\n" >> io($logfile);
	addlog($logfile, "Starting Nginx-cfgi");
	addlog($logfile, "Running with $> UID");
	addlog($logfile, "Perl $]");
	addlog($logfile, "Testing socket options");

	if ( ($unixsocket && $unixport) || (!($unixsocket) && !($unixport)) ) {
		print "\n\tERROR\tOnly one option can be used!\n";
		print "\tSuggested (beacuse of speed) is usage UNIX socket -S \n\n";
		exit 1;
	}

	if ($unixsocket) {
		print "       Daemon listening at UNIX socket $unixsocket\n" if $versbose;
		addlog($logfile, "Deamon listening at UNIX socket $unixsocket");
	} else {
		print "       Daemon listening at TCP/IP socket *:$unixport\n" if $verbose;

		addlog($logfile, "Daemon listening at TCP/IP socket *:$unixport");
	}

	if ( -e $filepid ) {
		print "\n\tERROR\t PID file $filepid already exists\n\n";
		addlog($logfile, "Can not use PID file $filepid, already exists.");
		exit 1;
	}

	if ( $unixsocket ) {
		print "       Creating UNIX socket\n" if $verbose;
		$socket = FCGI::OpenSocket( $unixsocket, 10 );
		if ( !$socket) {
			print "       Couldn't create socket\n";
			addlog($logfile, "Couldn't create socket");
			exit 1;
		}
		print "       Using UNIX socket $unixsocket\n" if $verbose;
	} else {
		print "       Creating TCP/IP socket\n" if $verbose;
		$portnumber = ":".$unixport;
		$socket = FCGI::OpenSocket( $unixport, 10 );
		if ( !$socket ) {
			print "       Couldn't create socket\n";
			addlog($logfile, "Couldn't create socket");
			exit 1;
		}
		print " Using port $unixport\n" if $verbose;
	}
	addlog($logfile, "Socket created");

	if ( ! $filepid ) {
		print "\n\tERROR\t PID file must declared\n"
		. "\tuse $0 with option -pid filename\n\n";
		exit 1;
	}
	print "       Using PID file $filepid\n" if $verbose;
	addlog($logfile, "Using PID file $filepid");

	my $pidnumber = $$;
	$pidnumber > io($filepid);
	print " PID number $$\n" if $verbose;
	addlog($logfile, "PID number $pidnumber");

}

sub addzero {
	my ($date) = shift;
	if ($date < 10) {
		return "0$date";
	}
	return $date;
}

sub logformat {
	my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$iddst) = localtime(time);
	my $datestring;
	$year += 1900;
	$mon++;
	$mon  = addzero($mon);
	$mday = addzero($mday);
	$min  = addzero($min);
	$datestring = "$year-$mon-$mday $hour:$min";
	return($datestring);
}

sub addlog {
	my ($log_file, $log_message) = @_;
	my $curr_time = logformat();
	my $write_message = "[$curr_time]   $log_message";
	$write_message >> io($log_file);
	"\n" >> io($log_file);
}

sub printerror {
	my $message = @_;
	print "\n   Nginx FastCGI\tERROR\n"
	. "\t $message\n\n";
	exit 1;
}

sub usage {
	print "\n   Nginx FastCGI \n"
	. "\n\tusage: $0 [-h] -S string -P int\n"
	. "\n\t-h\t\t: this (help) message"
	. "\n\t-S path\t\t: path for UNIX socket"
	. "\n\t-P port\t\t: port number"
	. "\n\t-p file\t\t: path for pid file"
	. "\n\t-l file\t\t: path for logfile"
	. "\n\n\texample: $0 -S /var/run/nginx-perl_cgi.sock -l /var/log/nginx/nginx-cfgi.log -pid /var/run/nginx-fcgi.pid\n\n";
	exit 1;
}


init;
#
END() { } BEGIN() { }
*CORE::GLOBAL::exit = sub { die "fakeexit\nrc=".shift()."\n"; }; eval q{exit};
if ($@) {
	exit unless $@ =~ /^fakeexit/;
} ;

# fork part
my $pid = fork();

if( $pid == 0 ) {
	&main;
	exit 0;
}

print " Forking worker process with PID $pid\n" if $verbose;
addlog($logfile, "Forking worker process with PID $pid");
print " Update PID file $filepid\n" if $verbose;
addlog($logfile, "Update PID file $filepid");
$pid > io($filepid);
print "       Worker process running.\n" if $verbose;
addlog ($logfile, "Parent process $$ is exiting");
exit 0;

sub main {
	$request = FCGI::Request( \*STDIN, \*STDOUT, \*STDERR, \%req_params, $socket );
	if ($request) { request_loop()};
	FCGI::CloseSocket( $socket );
}

sub request_loop {
	while( $request->Accept() >= 0 ) {
		# processing any STDIN input from WebServer (for CGI-POST actions)
		$stdin_passthrough = '';
		$req_len = 0 + $req_params{'CONTENT_LENGTH'};
		if (($req_params{'REQUEST_METHOD'} eq 'POST') && ($req_len != 0) ){
			while ($req_len) {
				$stdin_passthrough .= getc(STDIN);
				$req_len--;      
			}
		}

		# running the cgi app
		if ( (-x $req_params{SCRIPT_FILENAME}) &&
			(-s $req_params{SCRIPT_FILENAME}) &&
			(-r $req_params{SCRIPT_FILENAME})
		){
			foreach $key ( keys %req_params){
				$ENV{$key} = $req_params{$key};
			}
			if ( $verbose ) {
				addlog($logfile, "running $req_params{SCRIPT_FILENAME}");
			}
			# http://perldoc.perl.org/perlipc.html#Safe-Pipe-Opens
			#
			open $cgi_app, '-|', $req_params{SCRIPT_FILENAME}, $stdin_passthrough or print("Content-type: text/plain\r\n\r\n"); print "Error: CGI app returned no output - Executing $req_params{SCRIPT_FILENAME} failed !\n"; # addlog($logfile, "Error: CGI app returned no output - Executing $req_params{SCRIPT_FILENAME} failed !");

			if ($cgi_app) {
				print <$cgi_app>;
				close $cgi_app;
			}
		} else {
			print("Content-type: text/plain\r\n\r\n");
			print "Error: No such CGI app - $req_params{SCRIPT_FILENAME} may not exist or is not executable by this process.\n";
			addlog($logfile, "Error: No such CGI app - $req_params{SCRIPT_FILENAME} may not exist or is not executable by this process.");
		}
	}
}
