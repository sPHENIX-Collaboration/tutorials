#!/usr/bin/perl

use strict;
use warnings;

if ($#ARGV < 0)
{
    print "extracts theta/phi/x0/lambda0 output from G4 matscan cmd from logfile\n";
    print "usage matscan_digest.pl <logfile>\n";
    exit(-1);
}
if (! -f $ARGV[0])
{
    die "could not locate $ARGV[0]\n";
}

open(F,"$ARGV[0]");
open(F1,">matscan.dat");
my $foundthetaphi = 0;
my $skip =0;
while(my $line = <F>)
{
    chomp $line;

    if ($line =~ /Theta/ && $line =~ /Phi/ && $line =~ /lambda0/)
    {
	$foundthetaphi = 1;
	$skip = 0;
	next;
    }
    if ($line =~ /ave/)
    {
	$foundthetaphi = 0;
	next;
    }
    if ($line =~ /All done/)
    {
      last;
    }
    if ($foundthetaphi == 1)
    {
	if ($skip == 0)
	{
	    my @sp1 = split(/ /,$line);
#	    print "$line\n";
#	    print "no arr: $#sp1\n";
	    if ($#sp1 < 3)
	    {
		next;
	    }
	}
	$line =~ s/\s+/ /g; # replace all multiple spaces with single space
	print F1 "$line\n";
#	$foundthetaphi = 0;
	$skip = 0;
    }
}

close(F);
close(F1);
