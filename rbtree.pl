#!/usr/bin/env perl

package Report;

use strict;
use warnings;

use Carp;
use POSIX;
use Data::Dumper;

$Report::defaults = {
        debug           => 1,
        headers         => [],
        rows            => [],
        fieldSeparator  => '|',
        cornerChar      => '+',
        rowWidth        => 0,
        output          => "",
        pad             => 1,
        headerSeparator => 1
};

sub new {
        my $package = shift;
        my $self = bless(
                Report::merge(
                        $Report::defaults,
                        { @_ }
                ),
                $package
        );

        $self;
}

sub addHeaders {
        my ($self, $headers) = @_;

        # Check for a reference
        if(ref $headers) {
                foreach my $header (@{$headers}) {
                        push($self->{headers}, $header);
                }
        } else {
                croak "Not an array reference.\n";
        }

        $self->{headerCount} = scalar(@{$self->{headers}});
        $self->{colCount}    = scalar(@{$self->{headers}});

        $self;
}

sub addRow {
        my ($self, $row) = @_;

        croak "Not an array reference.\n" unless ref $row;

        if(scalar(@{$row}) > $self->{colCount}) {
                croak "Row contains more columns than there are headers\n";
        }

        push(@{$self->{rows}}, $row);

        $self->{rowCount} = scalar(@{$self->{rows}});

        $self;
}

sub addRows {
        my ($self, $rows) = @_;

        if(ref $rows) {
                foreach my $row(@{$rows}) {
                        croak "Not an array reference.\n" unless ref $row;
                        push($self->{rows}, $row);
                }
        } else {
                croak "Not an array reference.\n";
        }


        $self;
}

sub Render {
        my $self = shift;

        # Extract some info from our row data
        $self->_getMeta();

        # Assmeble format string
        $self->_assembleFormat();

        # Print top closure
        $self->{output} .= $self->_getClosure();

        # Format headers
        if($self->{pad}) {
                for(my $i = 0; $i <= ($self->{headerCount} - 1); $i++) {
                        $self->{headers}[$i] = $self->_pad(
                                $self->{headers}[$i],
                                ($self->{fieldStats}[$i] + 2)
                        );

                        push(@{$self->{separatorData}}, '-' x ($self->{fieldStats}[$i] + 2));
                }
        }
        # Print Headers
        $self->{output} .= sprintf($self->{format}, @{$self->{headers}});

        # Print Header Separator
        if($self->{headerSeparator}) {
                $self->{output} .= sprintf($self->{format}, @{$self->{separatorData}});
        }

        @{$self->{rows}} = sort {
            $b->[1] cmp $a->[1]
        } @{$self->{rows}};

        # Print Rows
        for(my $i = 0; $i <= ($self->{rowCount} - 1); $i++) {
                if($self->{pad}) {
                        my $cols = (scalar(@{$self->{rows}[$i]}));
                        for(my $f = 0; $f <= (scalar(@{$self->{rows}[$i]}) - 1); $f++) {
                                $self->{rows}[$i][$f] = $self->_pad(
                                        $self->{rows}[$i][$f],
                                        ($self->{fieldStats}[$f] + 2)
                                );
                        }
                }

                $self->{output} .= sprintf($self->{format}, @{$self->{rows}[$i]});
        }

        #Print bottom closure
        $self->{output} .= $self->_getClosure();

        return $self->{output};
}

sub _assembleFormat {
        my $self = shift;

        $self->{format} = '';
        for(my $i = 0; $i <= ($self->{colCount} - 1); $i++) {
                $self->{format} .= $self->{fieldSeparator};
                $self->{format} .= "%".($self->{fieldStats}[$i] + 2)."s";
        }


        $self->{format} .= "|\n";
}

sub _getMeta {
        my $self = shift;

        $self->{fieldStats} = [ ];
        for(my $i = 0; $i <= (scalar(@{$self->{headers}}) - 1); $i++) {
                my $fieldLen = 0;

                $fieldLen = length($self->{headers}[$i]);
                for(my $n = 0; $n <= ($self->{rowCount} - 1); $n++) {
                        my $flen = length($self->{rows}[$n][$i]);
                        $fieldLen = $flen > $fieldLen ? $flen : $fieldLen;
                }

                push(@{$self->{fieldStats}}, $fieldLen)
        }

        # Count total row width for closures
        $self->{rowWidth} += ( $_ + 2 ) for(@{$self->{fieldStats}});
        # Add number of columns + 1 for the trailing fieldSeparator in sprintf format
        $self->{rowWidth} += ($self->{headerCount} + 1);
}

sub _getClosure {
        my $self = shift;

        my $str = '';
        $str .= "+";
        $str .= "-" x ($self->{rowWidth} - 2);
        $str .= "+";
        $str .= "\n";

        return $str;
}

sub _pad {
        my ($self, $string, $width) = @_;

        my $pad = floor(($width - length($string)) / 2);

        my $str = "";
        $str .= " " x $pad;
        $str .= $string;
        $str .= " " x $pad;

        return $str;
}

###
# This function is lifted from Hash::Merge::Simple
# Trying to keep the module as 'self contained' as
# Possible.
# Credit goes to Robert Krimen <robertkrimen@gmail.com>
###
sub merge (@);
sub merge (@) {
  shift unless ref $_[0];
  my ($left, @right) = @_;

  return $left unless @right;

  return __PACKAGE__::merge($left, __PACKAGE__::merge(@right)) if @right > 1;

  my ($right) = @right;
  my %merge = %$left;

  for my $key (keys %{$right}) {
    my ($hr, $hl) = map { ref $_->{$key} eq 'HASH' } $right, $left;

    if($hr and $hl) {
      $merge{$key} = __PACKAGE__::merge($left->{$key}, $right->{$key});
    } else {
      $merge{$key} = $right->{$key};
    }
  }

  return \%merge;
}

sub Dump {
        my $self = shift;
        print Dumper \$self;
        exit;
}

package main;

use Socket;
use Tree::RB qw[LUEQUAL LUGTEQ LULTEQ LUGREAT LULESS LUNEXT LUPREV];
use Data::Dumper;

my @cidr = (
    32,
    29,
    25,
    24,
    23,
    18,
    16
);


# Default comparator?
my $t = Tree::RB->new();
my $r = Report->new(
    headerSeparator => 1
);

$r->addHeaders([
    "Source",
    "Rule Match",
    "Rule Addr",
    "Rule Mask",
    "Rule CIDR"
]);

sub main {
    my @ips;

    # Create a fixed number of 'rules'
    # Jam them into a binary tree
    for(1..500) {
        my $ip = randIP();
        my $cidr = randCIDR();
        my $mask = calcNetmask($ip, $cidr);
        my $value = "$ip|$mask|$cidr";
        push(@ips, $ip);
        $t->put(ip2dec($ip), $value);
        # Paint the node red
    }

    # Walk the tree for IP's 'greater than the rule'
    # Example: 
    #   Rule - 192.168.1.0/24
    #   Source - 192.168.1.50
    for(1..200) {
        my $ip = $ips[$_];
        my $s = skewIP($ip);

        if(my %res = lookup($s)) {
            #print Dumper \%res;
            if($res{left}{node} =~ 'UNDEF') {
                $r->addRow([$s, 'UNDEF', 'UNDEF', 'UNDEF', 'UNDEF']);
            } else {
	            my ($addr, $mask, $cidr) = split(/\|/, $res{left}{node});
	
	            $r->addRow([
	                $s,
	                ($res{left}{match}{matched} > 0) ? 'YES' : 'NO',
	                $addr,
	                $mask,
	                $cidr
	            ]);
            }
        }
    }

    print $r->Render();
}

sub lookup {
    my $s = shift;
    my $ds = ip2dec($s);

    my %res;

    # QUERY
    $res{QUERY}{ip} = $s;
    $res{QUERY}{dec} = $ds;

    # LEFT NODE
    $res{left}{node} = $t->lookup($ds, LULTEQ);
    if(!defined($res{left}{node})) {
        $res{left}{node} = 'UNDEF';
    } else {
        my ($ip, $netmask) = (split(/\|/, $res{left}{node}))[0,1];
        $res{left}{dec} = ip2dec((split(/\|/, $res{left}{node}))[0]);
        $res{left}{match}{matched} = compare($s, $ip, $netmask);
    }

    # RIGHT NODE
    $res{right}{node} = $t->lookup($s, LUGTEQ);
    if(!defined($res{right}{node})) {
        $res{right}{node} = 'UNDEF';
    } else {
        my ($ip, $netmask) = (split(/\|/, $res{right}{node}))[0,1];
        $res{right}{dec} = ip2dec((split(/|/, $res{right}{node}))[0]);
        $res{right}{match}{matched} = compare($s, $ip, $netmask);
    }

    return %res;
}

sub compare {
    my ($source, $addr, $mask) = @_;

    my $netBin = inet_aton($addr);
    my $maskBin = inet_aton($mask);
    my $maskedNet = ($netBin & $maskBin);

    return ((inet_aton($source) & $maskBin) eq $maskedNet) ? 1 : 0;
}

sub skewIP {
    my ($ip, $down) = @_;

    my ($oct1, $oct2, $oct3, $oct4) = split(/\./, $ip);

    if($down) {

    } else {
        if(rand(100) % 2) {
            if(rand(100) % 2) {
                $oct2 = randRange(1, 254);
            }
        }

        if((time() + rand(100)) % 2) {
            $oct3 = randRange(1, 254);
        }

        if((time() + rand(100)) % 3) {
            $oct4 = randRange(1, 254);
        }
    }

    return "$oct1.$oct2.$oct3.$oct4";
}

sub randIP {
    my $addr = int rand 2**32;
    
    return join '.', unpack 'C4', pack 'N', $addr;
}

sub randCIDR {
    return $cidr[int rand($#cidr)];
}

sub randRange {
    my ($x, $y) = @_;
    return int(rand($y - $x)) + $x;
}

sub ip2dec {
        unpack N => pack CCCC => split /\./ => shift;
}

sub calcNetmask {
        my ($network, $netbit) = @_;

        my $_bit = ( 2 ** (32 - $netbit) ) - 1;

        my $full_mask = unpack("N", pack("C4", split(/\./, '255.255.255.255')));
        my $netmask   = join ('.', unpack("C4", pack("N", ( $full_mask ^ $_bit ))));

        return $netmask;
}

main();
