#!/usr/bin/perl -wT
use Mojolicious::Lite;

# Define a route and its corresponding handler
get '/' => sub {
    my $c = shift;
    $c->render(text => 'Hello, world!');
};

# Start the Mojolicious server
app->start;
