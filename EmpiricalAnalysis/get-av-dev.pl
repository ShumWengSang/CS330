#!perl -w

my @sizes=(100,200,300,400,500,750,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000);

foreach my $size ( @sizes ) {
    open IN, "<data.$size" or die "data.$size cannot be open";
#all data files are 1-liners
    my $line = <IN>; 
    close IN;
#create an array from string - space as delimeter
    my @data = split ' ', $line;

    my $av = 0; # average
    my $num = 0; # count
    foreach ( @data ) {
        $av += $_;
        ++$num;
    }
    $av /= $num;

    my $var = 0; #variance
    foreach ( @data ) {
        $var += ($_-$av)**2;
    }
    $var /= $num;
    my $dev = sqrt( $var ); # deviation = sqrt of variance
    print "($size, $av) +- ($dev,$dev)\n"; # format for pgfplot
}
