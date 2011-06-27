#!/usr/bin/perl -w
# converts an image from any format that imagemagick understands to a
# polar coordinates based embedded format.

use strict;

$| = 1;

my $hub_size = 8;
my $num_leds = 16;
my $target_device;
my $resample_pixels = 4;
my $num_slices = 256;
my $dither = 0;
my $colormap_file = "netscape:";


sub usage {
    print STDERR "Usage: $0 [<options>] <source file> <destination file>\n";
    print STDERR "$0 converts an image from any format that imagemagick\n";
    print STDERR "understands to a polar coordinates based embedded format.\n";
    print STDERR "\n";
    print STDERR "Options are:\n";
    print STDERR "  --hub-size <LEDs>:   specify the distance from the center of the wheel\n";
    print STDERR "           to the first LED.\n";
    print STDERR "  --dither         Dither resultant image\n";
    print STDERR "  --no-dither      Do not dither resultant image\n";
    print STDERR "  --resample-pixels <pixels>:\n";
    print STDERR "           Use <pixels>^2 pixels to do resampling\n";
    print STDERR "  --m132s:         Target a m132s (16 LEDs, 2 bits per color)\n";
    print STDERR "  --m464q:         Target a m464q (32 LEDs, 5 bits per color)\n";
    print STDERR "  --vidpro7:       Target a vidpro7 (32 LEDs, 5 bits per color)\n";
    print STDERR "    (one of --m132s or --m464q or --vidpro7 must be specified)\n";
    print STDERR "  --colormap:      GIF file to import colormap from\n";
    exit(1);
};

while (@ARGV && $ARGV[0] =~ /^-/) {
    my $arg = shift @ARGV;
    if ($arg eq '--hub-size') {
        &usage unless @ARGV;
        $hub_size = shift @ARGV;
    } elsif ($arg eq '--m132s') {
        $hub_size = 8;
        $num_leds = 16;
        $target_device = 'm132s';
    } elsif ($arg eq '--m464q') {
        $hub_size = 4.5;
        $num_leds = 32;
        $target_device = 'm464q';
    } elsif ($arg eq '--vidpro7') {
        #$hub_size = 4.666;
        $hub_size = 4;
        #$hub_size = 3.8;
        $num_leds = 32;
        $target_device = 'vidpro7';
    } elsif ($arg eq '--resample-pixels') {
        &usage unless @ARGV;
        $resample_pixels = shift @ARGV;
    } elsif ($arg eq '--dither') {
        $dither = 1;
    } elsif ($arg eq '--no-dither') {
        $dither = 0;
    } elsif ($arg eq '--colormap') {
        &usage unless @ARGV;
        $colormap_file = shift @ARGV;
    } else {
        print STDERR "Unknown argument: $ARGV[0]\n";
        &usage;
    }
}

&usage unless @ARGV == 2;
&usage unless defined($target_device);

my ($src_file, $dest_file) = @ARGV;

use Image::Magick;
my $srcimg = new Image::Magick;
my $res = $srcimg->ReadImage($src_file);
warn "$res" if ((!ref($res)) && "$res");
my $src_xsize = $srcimg->Get("width");
my $src_ysize = $srcimg->Get("height");
#$srcimg  $srcimg->Layers(method=>"coalesce", method=>"overlay");
#$srcimg->Set( page=>$src_xsize."x".$src_ysize."+0+0" );
$srcimg = $srcimg->Coalesce();
warn "$srcimg" if !ref($srcimg);
print "Source image geometry is $src_xsize by $src_ysize\n";

use constant PI    => 4 * atan2(1, 1);

my $center_x = int($src_xsize / 2);
my $center_y = int($src_ysize / 2);

my $rscale;
if ($center_x > $center_y) {
    # wider than it is tall. chop off left and right side.
    $rscale = $center_y / ($num_leds + $hub_size);
} else {
    # taller than it is wide. chop off top and bottom.
    $rscale = $center_x / ($num_leds + $hub_size);
}

print "Center is at: $center_x,$center_y Rscale: $rscale\n";
print "Hub width in pixels is ".($rscale * $hub_size)."\n";

# Set up header file image
#my $dest_name = $dest_file;
#$dest_name =~ s/^.*\///;
#$dest_name =~ s/[^A-Za-z0-9]//g;
#$dest_name =~ s/data//g;
open(DST_BIN, ">$dest_file") || die "$dest_file: $!";
binmode(DST_BIN);

my $dstimg = new Image::Magick;
$res = $dstimg->Set(size=>"${num_slices}x${num_leds}");
warn "$res" if "$res";
my $cmap = new Image::Magick;
unless(-e $colormap_file) {
  die("-E- couldn't find colormap file $colormap_file");
}
$res = $cmap->ReadImage($colormap_file); 
warn "$res" if "$res";

# generate a lookup table to find the index in the colortable

my $cmapcolors = $cmap->Get('colors');
if($cmapcolors < 2) {
  die("-E- failure getting colors");
}

my %colortable;

for (my $i=0; $i < $cmapcolors; $i++) {
   my($red, $green, $blue) = split(',', $cmap->Get("colormap[$i]")); 
   $red   = &tpixel($red);
   $green = &tpixel($green);
   $blue  = &tpixel($blue);
   #print "Added colormap color $red,$green,$blue = $i\n";

   $colortable{"$red,$green,$blue"} = $i;
}

	my($tmpfn) = "tmp722777";

for (my $frame = 0; $srcimg->[$frame]; $frame++) {
    # Set up preview image
    print "Processing frame $frame\n";
    $res = $dstimg->ReadImage("xc:black");
    warn "$res" if "$res";
    my $dstframe = $dstimg->[$frame];
    print "phase 1\n";
    #$srcimg->[$frame]->write("frame-$frame-partial.png");

    for (my $slice = 0; $slice < $num_slices; $slice++) {
        #print "slice $slice\n";
	#print "$slice...";
        for (my $led = ($num_leds - 1); $led >= 0; $led--) {
            my @new_pixel = (0, 0, 0, 1);  # 1 is alpha? imagemagick documentation isn't clear.
            for (my $resample_slice = 0; $resample_slice < $resample_pixels; $resample_slice++) {
                my $resampled_slice = $slice + ($resample_slice / $resample_pixels);
                for (my $resample_led = 0; $resample_led < $resample_pixels; $resample_led++) {
                    my $resampled_led = $led + ($resample_led / $resample_pixels);
                
                    my $theta = $resampled_slice * 2 * PI / $num_slices;
                    my $r = ($resampled_led + $hub_size) * $rscale;
                
                    # this also converts from image coordinates (x grows to the
                    # right, y grows down) to regular cartisian
                    # coordinates (x grows to the right, y grows up)
    
                    my $x = $center_x + $r * cos($resampled_slice * 2 * PI / $num_slices);
                    my $y = $center_y - $r * sin($resampled_slice * 2 * PI / $num_slices);
    
                    my @rgb = $srcimg->[$frame]->GetPixel(x=>int($x), y=>int($y));
    
                    for (my $color = 0; $color < 3; $color++) {
                        $new_pixel[$color] += $rgb[$color];
                    }
                }
            }
    
            for (my $color = 0; $color < 3; $color++) {
                $new_pixel[$color] /= $resample_pixels * $resample_pixels;
            }
    
            $res = $dstframe->SetPixel(x=>$slice, y=>$led, color=>\@new_pixel);
            #warn "$res" if "$res";

        }
    }

    print "phase 2\n";

# perlmagick core-dumps when calling Map() or Remap() so we have to
# use the command line tool to do those functions

#    if ($dither) {
#        #$dstframe->Map(image=>$cmap, dither=>"True");
#        $dstframe->OrderedDither(treshold=>"o8x8");
#    } else {
#    #  print("map\n");
#        $res = $dstframe->Map(image=>$cmap, dither=>"False");
#      warn "$res" if !ref($res);
#    }

     unlink("$tmpfn.png");
     unlink("$tmpfn.gif");
     $res = $dstframe->WriteImage("$tmpfn.png");
     warn "$res" if $res;
     my($cmd);
     if($dither) {
       $cmd = "/opt/local/bin/convert $tmpfn.png -ordered-dither o8x8 $tmpfn.gif";
     } else {
	# remap colors with dither disabled
       $cmd = "/opt/local/bin/convert $tmpfn.png +dither -remap $colormap_file $tmpfn.gif";
     }
     print("$cmd\n");
     #system("$cmd");
`$cmd`;
     $? && die("-E- failure running '$cmd'");
     my $dstframe2 = new Image::Magick;
     $res = $dstframe2->ReadImage("$tmpfn.gif");
     warn "$res" if ((!ref($res)) && "$res");
     #warn "$res" if !ref($res);

	print("phase 3\n");
    for (my $slice = 0; $slice < $num_slices; $slice++) {
      #print "slice $slice\n";
        #print "$slice...";
        for (my $led = ($num_leds - 1); $led >= 0; $led--) {
            if ($target_device eq 'm132s') {
            # 2 bits for each color
                my @new_pixel = $dstframe2->GetPixel(x=>$slice, y=>$led);
                my ($r, $g, $b);
                print DST_BIN chr($r << 4 | $g << 2 | $b);
            } elsif ($target_device eq 'm464q' || $target_device eq 'vidpro7') {
                my ($red,$green,$blue) = $dstframe2->GetPixel(x=>$slice, y=>$led);
                $red   = int($red   * 255 + 0.5);
                $green = int($green * 255 + 0.5);
                $blue  = int($blue  * 255 + 0.5);
                #$red   = &tpixel($red);
                #$green = &tpixel($green);
                #$blue  = &tpixel($blue);
                if (defined($colortable{"$red,$green,$blue"})) {
                    # 8 bits per pixel, colormapped
                    print DST_BIN chr($colortable{"$red,$green,$blue"});
                    #print "Mapped $red,$green,$blue to ",  $colortable{"$red,$green,$blue"}, 
                     # "at $slice,$led\n";
                } else {
                    die "Unable to map color $red,$green,$blue ($slice,$led)\n";
                }
            } else {
                die "target device $target_device not supported ";
            }
        }
    }
    #print "\n";
}

#my $out_fn = $frame ? "$dest_file-${frame}.png" : "$dest_file.png";
#my $out_fn = "$dest_file.gif";
#$res = $dstimg->write($out_fn);
#warn "$res" if "$res";

#print "Wrote preview of image data to $out_fn\n";
#print "\n";

close(DST_BIN);
     #unlink("$tmpfn.png");
     #unlink("$tmpfn.gif");

#print "(In preview image, hub is at the top and edge is at the bottom.)\n";

# convert 0..1 floating point to to [0, 2^$bits) integer
sub float_to_range {
    my ($val, $max) = @_;

    my $result = int($val * $max);
    if ($result < 0) {
        return 0;
    } elsif ($result >= $max) {
        return $max - 1;
    } else {
        return $result;
    }
};
sub float_to_bits {
    my ($val, $bits) = @_;

    return &float_to_range($val, 1 << $bits);
};

sub tpixel {
  my ($pixel) = @_;

  return 255 * ($pixel/65535);

}

