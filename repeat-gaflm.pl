#!/usr/bin/perl -w

# Wrapper for running ga-flm several times.
#
# Also summarizes the ppl results of each ga-flm run into 1) summary
# file, 2) logfile, and 3) timelog file

# Usage: perl repeat-gaflm.pl [number of GA runs] [ga-flm options]
# Examples:
#     perl repeat-gaflm.pl     => runs gaflm once
#     perl repeat-gaflm.pl 3   => runs gaflm 3 times
#     perl repeat-gaflm.pl 2 -f myflmfile 
#             => runs "gaflm -f myflmfile" twice

use strict;

# get options
my $num_runs = 1;
my $options = "";
my $gaparamfile = "GA-PARAMS"; # default filename for GA param
my $next_is_gaparam = 0;
if (@ARGV){
    if ($ARGV[0] =~ /\d+/){
	$num_runs = shift @ARGV;
    }
    while (@ARGV){
	my $opt = shift @ARGV;
	$options .= "$opt ";
	if ($opt =~ /\-g/){
	    $next_is_gaparam = 1;
	}
	elsif ($next_is_gaparam == 1){
	    $gaparamfile = $opt;
	    $next_is_gaparam = 0;
	}
    }
}


# get PATH_for_GA_FLM_files from GA-PARAM.
my $path;
open GAPARAM, "$gaparamfile" or die "Cannot open $gaparamfile: $!\n";
foreach my $l (<GAPARAM>){
    my $tmp;
    if ($l =~ /^PATH/){
	($tmp, $path) = split /\=/, $l;
	$path =~ s/\s//g;
    }
}

########################################
# Beginning to run GA-FLM here

print "Running ga-flm $num_runs times...\n";
system("rm -v ${path}timelog");
open TIMELOG, ">>${path}timelog" or die "Cannot open timelog\n";;

# loop for running gaflm
foreach my $run (1..$num_runs){
    my $com = "./ga-flm $options";

    # recording the time in timelog
    my $time = `date`;
    print TIMELOG "Run $run: $time";

    # remove unwanted files and run ga-flm
    system("rm -fv ${path}*.EvalLog; rm -fv ${path}*.complexity; rm -fv ${path}*.flm;");
    system($com);

    # summarize results for this run
    system("mv -v ${path}logfile.txt ${path}logfile$run");
    &summarize($path,$run);
}
close TIMELOG;
print "\a"x2; # beeps to signal finish

###################################################################

# Summarize function: extract PPL values from EvalLog files and concat
# it with the corresponding flm file to create a list of easy-to-read
# report file.
sub summarize{
    my $path = shift;
    my $run = shift;
    open OUT, ">${path}summary$run" or die "Cannot open file $! in summarize\n";
    my %allppl = (); # records all ppl values

    foreach my $file (<${path}*.EvalLog>){

	my $flm = $file;
	$flm =~ s/\.EvalLog/.flm/;
	print OUT "======= $flm =======\n\n";

	# print OUT ppl of this evallog
	open (my $evalfilefh, "<", $file)  or die "can't open '$file': $!\n";
	my @evallines = <$evalfilefh>;

	$evallines[$#evallines] =~ /ppl= (\d+.\d+) /;
	my $ppl = $1;
	print OUT "ppl= $ppl \n\n";

	close $evalfilefh or die "can't close $evalfilefh: $!\n";

	# print OUT corresponding flmfile
	open (my $flmfh, "<", $flm)  or die "can't open '$flm': $!\n";
	while (<$flmfh>) {
	    print OUT;
	}
	close $flmfh or die "couldn't close '$flm': $!\n";
	print OUT "\n\n";

	# store ppl and corresponding flm filename in a hash for
	# overall summary
	$allppl{$ppl}=$flm;

    }

    foreach (sort {$b <=> $a} keys %allppl){
	print OUT "ppl= " . $_ . "\t" . $allppl{$_} . "\n";
    }
    close OUT;
}



