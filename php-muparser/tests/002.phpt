--TEST--
Check for muparser presence
--FILE--
<?php 
$t = new mu\Parser();
var_export($t);
?>
--EXPECT--

