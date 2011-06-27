#!/usr/bin/perl -w

# TODO: convert to SerialPort packages see:
# http://www.perlmonks.org/?node_id=688080


my($PROGRAMDIR, $PROGRAMFILE);  # dir/file executed by user
BEGIN {
    ($PROGRAMDIR = $0) =~ s/([^\/]+)$//;
    $PROGRAMFILE = $1;
    if($PROGRAMDIR eq "") {$PROGRAMDIR = "."}

    push(@INC, $PROGRAMDIR);
}

my $srcdir = "$PROGRAMDIR";
my %cmds;
my %pats;
my %colortables;

my $comms_c = "$srcdir/comms_cmds_incl.c";
my $gfx_h = "$srcdir/gfx_colormap_incl.c";

sub usage {
    print STDERR "
Usage: $0 <serial device> ...

$0 outputs a sequence of commands (or arbitrary data), normally to a
serial device.  Command names are read from $comms_c.  Patterns are
read from directory $gfx_h.

Arguments may be:
  decimal numbers from 0 to 255
  hexadecimal numbers in the form 0xHH
  16-bit decimal numbers followed by \"s\"
  32-bit decimal numbers followed by \"l\"
  the name of a command in $comms_c
  \"/\" followed by the name of a pattern in $gfx_h
  \".\" followed by an ascii string to send

Multi-byte numbers are transmitted big endian.

For example, to switch to the dead battery pattern, you could use:

$0 /dev/cu.usbserial-A1001MPI pat /dead

";
    if (%cmds) {
        print STDERR "Known command codes are:\n";
        for my $cmd (keys %cmds) {
            print STDERR "$cmd -> $cmds{$cmd}\n";
        }
        print STDERR "\n";
    }

    if (%pats) {
        print STDERR "Known patterns:\n";
        for my $pat (keys %pats) {
            print STDERR "$pat -> $pats{$pat}\n";
        }
        print STDERR "\n";
    }

    if (%colortables) {
        print STDERR "Known colortables:\n";
        for my $col (keys %colortables) {
            print STDERR "$col -> $colortables{$col}\n";
        }
        print STDERR "\n";
    }
    exit(1);
};

my $quiet=0;

while (@ARGV && $ARGV[0] =~ /^-/) {
    my $arg = shift @ARGV;
    if ($arg eq '--srcdir') {
        &usage unless @ARGV;
        $srcdir = shift @ARGV;
    } elsif ($arg eq "-") {
        $comport = "-";
        ### this is ok, will be tested for later.
    } elsif ($arg eq "-q") {
        $quiet=1; 
    } else {
        print STDERR "Unknown argument: $ARGV[0]\n";
        &usage;
    }
}

if (!open(COMMS_C, "$comms_c")) {
    warn "cannot open $comms_c: $!";
} else {
    while (<COMMS_C>) {
        if (/^\s*CMD_([a-zA-Z_]+)\s*=\s*(0x[0-9a-fA-F]*),?\s*$/) {
            $cmds{$1} = $2;
        }
    }
    close(COMMS_C);
}

if (!open(GFX_H, "$gfx_h")) {
    warn "cannot open $gfx_h: $!";
} else {    
    while (<GFX_H>) {
        if (/^\s*COLORMAP_(\w+)\s*=\s*([0-9]*),?\s*$/) {
            $colortables{$1} = $2;
        }
        if(/define PATTERN_ID_(\S+)\s+(\d+)/) {
        	$pats{lc($1)} = $2;  # pattern name, pattern number
        	#print("got pattern $1 $2\n");
        }
    }
    close(GFX_H);
}


if (! defined($comport)) {
    &usage unless @ARGV > 1;
    $comport = shift @ARGV;
}

my $parse_errors = 0;
@to_send = ();

my($argcount) = 0;
my($sendlen) = 0;

for my $arg (@ARGV) {
    my $to_send;
    my ($cmd, $pat, $col);
    my $xtra;

    $argcount++;
    if ($arg =~ /^([0-9]+)$/ && $1 < 256) {
        $to_send = chr($1);
    } elsif ($arg =~ /^0x([0-9a-f][0-9a-f])/i) {
        $to_send = chr(hex($1));
    } elsif ($arg =~ /^\.(.*)$/) {
        $to_send = $1;
    } elsif ($col = &match(\%colortables, $arg)) {
        $to_send = chr($colortables{$col});
        $xtra = $col;
    } elsif ($cmd = &match(\%cmds, $arg)) {
        $to_send = chr(hex($cmds{$cmd}));
        $xtra = $cmd;
    } elsif ($arg =~ /^\/(.*)$/ && ($pat = &match(\%pats, $1))) {
        $to_send = chr($pats{$pat});
        $xtra = $pat;
    } elsif ($arg =~ /^([0-9]+)s/) {
        $to_send = chr($1 >> 8) . chr($1 & 0xFF);
    } elsif ($arg =~ /^([0-9]+)l/) {
        $to_send = chr($1 >> 24) . chr(($1 >> 16) & 0xFF) .
                   chr(($1 >> 8) & 0xFF) . chr($1 & 0xFF);
    } else {
        printf STDERR "%-40s %s\n", $arg, "Could not parse";
        $parse_errors++;
    next;
    }

    
    if ($xtra) {
        $arg .= " ($xtra)";
    }
    $arg .= ":";
    
    if (!$quiet) {
        printf STDERR "%-40s", $arg;

        for (my $pos = 0; $pos < length($to_send); $pos++) {
            printf STDERR " %02X", ord(substr($to_send, $pos, 1));
        }
        print STDERR "\n";
    }
    $sendlen += length($to_send);
    push(@to_send, $to_send);
}
#print("total args: $argcount\n");
#print("total command script size: $sendlen bytes\n");

if ($parse_errors || !@to_send) {
    print STDERR "\n";
    print STDERR "Parse errors occured.  Nothing sent\n";
    exit(1);
}

if ($comport eq "-") {

    binmode(STDOUT);

    for my $to_send (@to_send) {
        print STDOUT $to_send;
    }

} else {
    open(SERIAL, ">>$comport") || die "$comport: $!";
    binmode(SERIAL);

    for my $to_send (@to_send) {
        print SERIAL $to_send;
    }
    close(SERIAL) || warn "closing $comport: $!";
}

if (!$quiet) { print STDERR "Done writing to $comport\n"; }

sub match {
    my ($table, $arg) = (@_);
    my $nmatch = 0;
    my @matched_entry;

    for my $entry (keys %$table) {
        if ($entry =~ /^$arg$/i) {
            $nmatch++;
            push(@matched_entry, $entry);
        }
    }
    if ($nmatch > 1) {
        printf STDERR "%-40s %s\n", $arg, "Ambiguous: (".join(",",@matched_entry).")";
        return undef;
    }
    if (@matched_entry) {
        return $matched_entry[0];
    } else {
        return undef;
   }
};
