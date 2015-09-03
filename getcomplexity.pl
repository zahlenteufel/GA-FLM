#!/usr/nikola/bin/perl -w

# computes number of parameters for a LM file
# usage: getcomplexity.pl < a.lm > a.complexity

my $sum = 0;
LINE: while (my $l = <>){
    if ($l =~ /\\0x0\-grams/){
	last LINE;
    }
    elsif ($l =~ /ngram/){
	my ($ngram, $count) = split /=/, $l;
	$sum += $count;
    }
}
print "$sum\n";
