#===============================================================================
#
#         FILE: Report.pm
#
#  DESCRIPTION: Generate simple ASCII reports.
#
#         BUGS: A couple
#        NOTES: I need to add comments
#       AUTHOR: Brandon B
#      VERSION: 1.0
#      CREATED: 05/21/2014 07:04:27 PM
#===============================================================================

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














1;
