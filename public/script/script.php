<?php


$fh = fopen('php://stdin', 'r');
stream_set_blocking($fh, false);
$stdin = fgets($fh);
echo "stdin: '$stdin'";  
?>
