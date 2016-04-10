#!/usr/bin/perl
use strict;

my $ccodeold = undef;

while (<>) {
	my $str = $_; 
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $c2sym = $1; $c2sym =~ s/"//g;
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $cname = $1; $cname =~ s/"//g;
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $opname = $1; $opname =~ s/"//g;
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $ccode = $1; $ccode =~ s/"//g;
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $opcodes = $1; $opcodes =~ s/"//g;
	$str =~ /([^,]*){0,1},/; $str = $'; 
	my $plmin = $1; $plmin =~ s/"//g;
	$str =~ /([^,]*){0,1},/; $str = $'; 
	my $plmax = $1; $plmax =~ s/"//g;
	$str =~ /([^,]*){0,1},/; $str = $'; 
	my $mcc = $1; $mcc =~ s/"//g;
	$str =~ /("[^,]*"){0,1},/; $str = $'; 
	my $mnc = $1; $mnc =~ s/"//g;
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $clx = $1; $clx =~ s/"//g; $clx =~ s/,/\./g; if ( $clx == "" ) { $clx=-1; }
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $mt_life = $1; $mt_life =~ s/"//g; $mt_life =~ s/,/\./g; if ( $mt_life== "" ) { $mt_life=-1; }
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $infobip = $1; $infobip =~ s/"//g; $infobip =~ s/,/\./g; if ( $infobip == "" ) { $infobip=-1; }
	$str =~ /("[^"]*"){0,1},/; $str = $';
	my $nth = $1; $nth =~ s/"//g; $nth =~ s/,/\./g; if ( $nth == "" ) { $nth=-1; }
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $mt_routesms = $1; $mt_routesms =~ s/"//g; $mt_routesms =~ s/,/\./g; if ( $mt_routesms == "" ) { $mt_routesms=-1; }
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $mt_beepsend = $1; $mt_beepsend =~ s/"//g; $mt_beepsend =~ s/,/\./g; if ( $mt_beepsend == "" ) { $mt_beepsend=-1; }
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $mt_kievstar = $1; $mt_kievstar =~ s/"//g; $mt_kievstar =~ s/,/\./g; if ( $mt_kievstar == "" ) { $mt_kievstar=-1; }
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $mt_as = $1; $mt_as =~ s/"//g; $mt_as =~ s/,/\./g; if ( $mt_as == "" ) { $mt_as=-1; }
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $mt_up = $1; $mt_up =~ s/"//g; $mt_up =~ s/,/\./g; if ( $mt_up == "" ) { $mt_up=-1; }
	$str =~ /("[^"]*"){0,1},/; $str = $'; 
	my $mt_motiv = $1; $mt_motiv =~ s/"//g; $mt_motiv =~ s/,/\./g; if ( $mt_motiv == "" ) { $mt_motiv=-1; }
	

	if ( $ccode == undef ) {
		$ccodeold = $ccode;
	}


	if ( $ccode != $ccodeold ) {
		print "$ccode``$ccode``$c2sym``$c2sym:unknown``$c2sym:unknown``$cname``0\n";
		$ccodeold = $ccode;
	}

	if ( $opcodes != "" ) {

		my @opcode = split /,/, $opcodes;
		foreach ( @opcode ) {
			print "$ccode$_``$ccode``$c2sym``$c2sym:$mcc:$mnc``$opname``$cname``0``mt_clx=$clx&mt_life=$mt_life&mt_ib=$infobip&mt_nth=$nth&mt_route=$mt_routesms&mt_beep=$mt_beepsend&mt_uaks=$mt_kievstar&mt_as=$mt_as&mt_up=$mt_up&mt_motiv=$mt_motiv\n";
		}

	} else {
		print "$ccode``$ccode``$c2sym``$c2sym:$mcc:$mnc``$opname``$cname``0``mt_clx=$clx&mt_life=$mt_life&mt_ib=$infobip&mt_nth=$nth&mt_route=$mt_routesms&mt_beep=$mt_beepsend&mt_uaks=$mt_kievstar&mt_as=$mt_as&mt_up=$mt_up&mt_motiv=$mt_motiv\n";
	}

}
