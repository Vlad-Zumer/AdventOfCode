# perl -W main.pl

use strict;
use Data::Dumper;

use constant false => 0;
use constant true  => 1;

sub readInput {
    my $fileName = $_[0];

    my @output;
    open(FH, '<', $fileName) or die "File '$fileName' can't be opened"; 

    while(<FH>)
    {
        # make int from each element from the split line
        my @nums = map {int($_)} split(' ', $_);
        push(@output, \@nums);
    }

    close(FH);
    return @output;
}

sub sign {
    my $val = $_[0];
    if ($val < 0)
    {
        return -1;
    } 
    elsif ($val > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

sub checkReport {
    my @report = @_;
    my $ok = true;
    my $prevSign = 0;

    for(1 .. $#report)
    {
        my $res = @report[($_ - 1)] - @report[($_)];
        my $sigRes = sign($res);
        $res = abs($res);
        if ($sigRes == 0 || ($sigRes + $prevSign) == 0)
        {
            $ok = false;
            last;
        }
    
        $prevSign = $sigRes;
    
        if ($res > 3 || $res < 1) {
            $ok = false;
            last;
        }
    }

    return $ok;
}

sub p1 {
    my @reports = @_;

    my $sum = 0;
    for(@reports)
    {
        my @report = @$_;
        # print "report: @report\n";
        my $ok = checkReport(@report);
        if ($ok)
        {
            $sum = $sum + 1;
        }
    }

    return $sum;
}

sub p2 {
    my @reports = @_;

    my $sum = 0;
    for(@reports)
    {
        my @report = @$_;
        # print "report: @report\n";

        my $ok = checkReport(@report);
        my $index = 0;
        while (!$ok && $index <= $#report)
        {
            my @subReport = @report;
            splice @subReport, $index, 1;
            # print "  subReport: @subReport\n";
            $ok = checkReport(@subReport);
            $index = $index + 1;
        }

        if ($ok) 
        {
            $sum = $sum + 1;
        }

    }

    return $sum;
}

# my $fileName = 'test.txt';
my $fileName = "input.txt";

my @reports = readInput($fileName);
# print Dumper(@reports);

print "p1=", p1(@reports), "\n";
print "p2=", p2(@reports), "\n";

print "DONE";