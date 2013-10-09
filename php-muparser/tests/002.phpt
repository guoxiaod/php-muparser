--TEST--
muparser test define const
--FILE--
<?php 
$t = new mu\Parser();
$t->DefineConst("H", 923);
$t->DefineConst("Y", 235.2);

$r = $t->GetConst();

echo "\n";
var_export($r);
echo "\n";

$data = array(
    'a' => 0,
    'b' => 0,
    'c' => 0,
    'd' => 0
    );

$t->DefineVar($data);

$t->SetExpr("a=H+Y,b=H-Y,c=H*Y,d=H/Y");

$r = $t->Eval($n);

echo "\n";
var_export($r);
echo "\n";

$n = 0;
$t->Eval($n);

echo "\n";
var_export($t->GetVar());
echo "\n";

$r = $t->GetConst();

echo "\n";
var_export($r);
echo "\n";

?>
--EXPECT--

array (
  'H' => 923,
  'Y' => 235.2,
  '_e' => 2.718281828459,
  '_pi' => 3.1415926535898,
)

array (
  0 => 1158.2,
  1 => 687.8,
  2 => 217089.6,
  3 => 3.9243197278912,
)

array (
  'a' => 1158.2,
  'b' => 687.8,
  'c' => 217089.6,
  'd' => 3.9243197278912,
)

array (
  'H' => 923,
  'Y' => 235.2,
  '_e' => 2.718281828459,
  '_pi' => 3.1415926535898,
)
