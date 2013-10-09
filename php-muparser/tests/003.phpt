--TEST--
muparser test define var
--FILE--
<?php 
$t = new mu\Parser();
$t->DefineVar("H", 923.1);
$t->DefineVar("Y", 235.2);

$r = $t->GetVar();

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

?>
--EXPECT--

array (
  'H' => 923.1,
  'Y' => 235.2,
)

array (
  0 => 1158.3,
  1 => 687.9,
  2 => 217113.12,
  3 => 3.9247448979592,
)

array (
  'H' => 923.1,
  'Y' => 235.2,
  'a' => 1158.3,
  'b' => 687.9,
  'c' => 217113.12,
  'd' => 3.9247448979592,
)
