#!/usr/bin/perl 

use Image::Magick;

my($image);
my($colorflip) = 0;
my($redmax) = 63;
my($greenmax) = 63;
my($bluemax) = 63;
my($maxin) = 255;  # upper limit of values we'll be translating
my($table_name);

sub usage {
  print STDERR "Usage $0: <source file>\n";
  print STDERR "$0 takes the colormap found in a 256-color image, transforms it,\n";
  print STDERR "and writes the result to STDOUT in the form of assembly.\n";
  exit(1);
}

my (@reverse_table) = (
    1, 129, 65, 193, 33, 161, 97, 225, 17, 145, 81, 209, 49, 177, 113,
    241, 9, 137, 73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185,
    121, 249, 5, 133, 69, 197, 37, 165, 101, 229, 21, 149, 85, 213, 53,
    181, 117, 245, 13, 141, 77, 205, 45, 173, 109, 237, 29, 157, 93,
    221, 61, 189, 125, 253, 3, 131, 67, 195, 35, 163, 99, 227, 19, 147,
    83, 211, 51, 179, 115, 243, 11, 139, 75, 203, 43, 171, 107, 235,
    27, 155, 91, 219, 59, 187, 123, 251, 7, 135, 71, 199, 39, 167, 103,
    231, 23, 151, 87, 215, 55, 183, 119, 247, 15, 143, 79, 207, 47,
    175, 111, 239, 31, 159, 95, 223, 63, 191, 127, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255 
);

while (@ARGV && $ARGV[0] =~ /^-/) {
    my $arg = shift @ARGV;
    if ($arg eq '--redmax') {
        &usage unless @ARGV;
        $redmax = shift @ARGV;
    } elsif ($arg eq '--greenmax') {
        &usage unless @ARGV;
        $greenmax = shift @ARGV;
    } elsif ($arg eq '--bluemax') {
        &usage unless @ARGV;
        $bluemax = shift @ARGV;
    } elsif ($arg eq '--colormax') {
        my($tmp);
        &usage unless @ARGV;
        $tmp = shift @ARGV;
        $redmax = $tmp;
        $greenmax = $tmp;
        $bluemax = $tmp;
    } elsif ($arg eq '--name') {
        &usage unless @ARGV;
        $table_name = shift @ARGV;
    } elsif ($arg eq '--colorflip') {
        $colorflip=1;
    } else {
        print STDERR "Unknown argument: $ARGV[0]\n";
        &usage;
    }
}

&usage unless @ARGV == 1;

my($src_file) = @ARGV; 

if (!$table_name) {
    $table_name = `basename $src_file` ; # strip any path information
    chomp $table_name;
    $table_name =~ s/\..*//g;   # strip file suffix
}

$image = Image::Magick->new;
my $res = $image->Read($src_file);
die $res if $res;

my $col = $image->Get('colors');

my (@reds, @greens, @blues);

for (my $j=0; $j < $col; $j++) {
    my($color) = $image->Get("colormap[$j]");
    if ($color) {
        my($red, $green, $blue) = split(',',$color);
        $red = 255 * ($red / 65535);
        $green = 255 * ($green / 65535);
        $blue = 255 * ($blue / 65535);
        $redconv = &redadjust($red);
        $greenconv = &greenadjust($green);
        $blueconv = &blueadjust($blue);

        #print STDERR "Converted ($red, $green, $blue) to ($redconv, $greenconv, $blueconv)\n";
        
        push @reds, $redconv; 
        push @greens, $greenconv; 
        push @blues, $blueconv;
    } else {
        die "Got undefined color value at index $j. (make sure any PNGs are 8-bit only)\n";
    }
}

for (my $j=0; $j < (256 - $col); $j++) {
    push @reds, &redadjust(0);
    push @greens, &greenadjust(0);
    push @blues, &blueadjust(0);
}


print "\t.global $table_name\n";
print "\t.section .colormaps\n";
print "\t.size $table_name, 768\n\n";

print "$table_name:\n";


print "; red values\n";
&printmap(@reds);

print "; green values\n";
&printmap(@greens);

print "; blue values\n";
&printmap(@blues);

sub printmap {
    my (@map) = @_;

    for (my $row=0; $row < 16; $row++) {
        print "\t.byte ";
        my ($comma) = ", ";
        for (my $i=0; $i < 16; $i++) {
            if($i == 15) { $comma = ""; }
            print $map[$row * 16 + $i], $comma;
        }
        print "\n";
    }
}

sub color_fix {
    my ($color) = @_;

    if ($colorflip) {
        return $color;
    } else {
        # set the high bit on color by adding 128. 
        return $color + 128;
    }
}

sub redadjust {
    my ($in) = @_;
    my ($red);

    # normalize to value between 0 and 1
    $red = -0.00088576 + 0.0107257 * 2.63421**(((5.32107 * $in)/$maxin) -0.637948 );
    # scale to desired range and round
    $red = int( $red * $redmax + 0.5);

    if ($red > $redmax) {
        $red = $redmax;
    }

    if ($in == 0) {   # make sure there's a black
        $red = 0;
    }

    return &color_fix($red);
}

sub greenadjust {
    my ($in) = @_;
    my ($green);

    # normalize to value between 0 and 1
    $green = -0.00335187 + 0.00791497 * 2.4215**(((5.19307 * $in)/$maxin) - 0.295047);
    # scale to desired range and round
    $green = int( $green * $greenmax + 0.5);

    if ($green > $greenmax) {
        $green = $greenmax;
    }

    if ($in == 0) {   # make sure there's a black
        $green = 0;
    }

    return &color_fix($green);

}

# Use same transformation for blue as green.
sub blueadjust {
    my ($in) = @_;

    # normalize to value between 0 and 1
    $blue = -0.00335187 + 0.00791497 * 2.4215**((5.19307 * $in)/$maxin - 0.295047);
    # scale to desired range and round
    $blue = int( $blue * $bluemax + 0.5);

    if ($blue > $bluemax) {
        $blue = $bluemax;
    }

    if ($in == 0) {   # make sure there's a black
        $blue = 0;
    }

    return &color_fix($blue);

}

