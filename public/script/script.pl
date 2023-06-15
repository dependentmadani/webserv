#!/usr/bin/env perl

use strict;
use warnings;
use CGI;

hhhh

print "$ENV{REDIRECT_STATUS} 200 OK\n";
print "Server: $ENV{SCRIPT_FILENAME}\n";
print "Content-type: text/plain\n\n";
print "OK, starting time consuming process ... \n";
# Tell Perl not to buffer our output
 $| = 1;
 for ( my $loop = 1; $loop <= 30; $loop++ )
 {
    print "Iteration: $loop\n";
    ## Perform some time consuming task here ##
    sleep 1;
}
print "All Done!\n";