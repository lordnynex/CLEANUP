#!/usr/bin/env perl

use strict;
use warnings;

use POSIX;
use Socket;
use Data::Dumper;

my $ip = $ARGV[0];
my $cidr = $ARGV[1];

sub _calc_netmask {
        my ($network, $netbit) = @_;

        my $_bit = ( 2 ** (32 - $netbit) ) - 1;

        my $full_mask = unpack("N", pack("C4", split(/\./, '255.255.255.255')));
        my $netmask   = join ('.', unpack("C4", pack("N", ( $full_mask ^ $_bit ))));

        return $netmask;
}

sub _ip2bin {
        my $ip = shift;

        return sprintf("%s %s %s %s", unpack("B8B8B8B8", inet_aton($ip)));
}

sub _bin2ip {
        my $bin = shift;
        my @parts = split(/\s/, $bin);

        if($#parts ne 3) {
                warn "_bin2ip recieved something strange\n";
                print Dumper \@parts;
                die;
        }

        for(my $i = 0; $i <= $#parts; $i++) {
                $parts[$i] = oct("0b".$parts[$i]);
        }

        return join(".", @parts);
}

sub _ip2hex {
        my $ip = shift;
        #return sprintf("%02x%02x%02x%02x", $ip);
        return sprintf("%02x"x4, unpack("C4", inet_aton($ip)));
}

sub _hex2ip {
        my $hex = shift;
        return inet_ntoa( pack("N", hex($hex)));
}

sub _ip2dec {
        unpack N => pack CCCC => split /\./ => shift;
}

sub _dec2ip {
        join '.', unpack 'C4', pack 'N', shift;
}

sub _findSubnetAddr {
        my ($ip, $subnet) = @_;

        return(
                _bin2ip(
                        (
                                _ip2bin($ip)
                                &
                                _ip2bin($subnet)
                        )
                )
        );
}

sub _findBroadcast {
        my ($subnetAddr, $netbit) = @_;

        my $bin = _ip2bin($subnetAddr); $bin =~ s/\s//g;

        my $bits = length($bin) - $netbit;
        substr($bin, -$bits, $bits, '1' x $bits);

        $bin =~ s/(.{8})/$1 /g;
        return _bin2ip($bin);
}

sub _pad {
        my($string, $width) = @_;

        my $pad = floor(($width - length($string)) / 2);

        my $str = " " x $pad;
        $str .= $string;
        $str .= " " x $pad;
        return $str;
}


my $col1 = 13;
my $col2 = 15;
my $col3 = 37;
my $col4 = 10;
my $col5 = 12;
my $format = "|%${col1}s|%${col2}s|%${col3}s|%${col4}s|%${col5}s|";

sub main {
        my @output;
        # Headers
        push(@output,
                sprintf(
                        $format,
                        " ",
                        _pad("DotQuad", $col2),
                        _pad("Binary" , $col3),
                        _pad("Hex"    , $col4),
                        _pad("Decimal", $col5)
                )
        );

        push(@output,
                sprintf(
                        $format,
                        "-" x $col1,
                        "-" x $col2,
                        "-" x $col3,
                        "-" x $col4,
                        "-" x $col5
                )
        );

        # User Supplied IP Address
        push(@output,
                sprintf(
                        $format,
                        _pad("IP", $col1),
                        _pad($ip, $col2),
                        _pad(_ip2bin($ip), $col3),
                        _pad(_ip2hex($ip), $col4),
                        _pad(_ip2dec($ip), $col5)
                )
        );

        # Netmask
        my $netmask = _calc_netmask($ip, $cidr);
        push(@output,
                sprintf(
                        $format,
                        _pad("Netmask", $col1),
                        _pad($netmask, $col2),
                        _pad(_ip2bin($netmask), $col3),
                        _pad(_ip2hex($netmask), $col4),
                        _pad(_ip2dec($netmask), $col5)
                )
        );

        # Subnet Host
        my $subnetAddr = _findSubnetAddr($ip, $netmask);
        push(@output,
                sprintf(
                        $format,
                        _pad("Subnet Host", $col1),
                        _pad($subnetAddr, $col2),
                        _pad(_ip2bin($subnetAddr),$col3),
                        _pad(_ip2hex($subnetAddr), $col4),
                        _pad(_ip2dec($subnetAddr), $col5)
                )
        );

        # Broadcast
        my $broadcast = _findBroadcast($subnetAddr, $cidr);
        push(@output,
                sprintf(
                        $format,
                        _pad("Broadcast", $col1),
                        _pad($broadcast, $col2),
                        _pad(_ip2bin($broadcast), $col3),
                        _pad(_ip2hex($broadcast), $col4),
                        _pad(_ip2dec($broadcast), $col5)
                )
        );

        my $close = '+';
        $close .= '-' x (length($output[1]) - 2);
        $close .= '+';
        unshift(@output, $close);
        push(@output, $close);
        print "$_\n" for(@output);
}

main();
