--TEST--
muparser test define const
--FILE--
<?php 

ini_set("precision", "13");
ini_set("serialize_precision", "13");

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
  '_pi' => 3.14159265359,
)

array (
  0 => 1158.2,
  1 => 687.8,
  2 => 217089.6,
  3 => 3.924319727891,
)

array (
  'a' => 1158.2,
  'b' => 687.8,
  'c' => 217089.6,
  'd' => 3.924319727891,
)

array (
  'H' => 923,
  'Y' => 235.2,
  '_e' => 2.718281828459,
  '_pi' => 3.14159265359,
)
