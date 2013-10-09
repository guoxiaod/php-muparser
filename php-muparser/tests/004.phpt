--TEST--
muparser test clear var
--FILE--
<?php 


$t = new mu\Parser();

// first define
$t->DefineVar("H", 923.1);
$t->DefineVar("Y", 235.2);

$r = $t->GetVar();

echo "\n";
echo "first define:\n";
var_export($r);
echo "\n";

$t->ClearVar();

// after clear
$r = $t->GetVar();
echo "\n";
echo "after clear:\n";
var_export($r);
echo "\n";

// define again
$t->DefineVar("H", 923.1);
$t->DefineVar("Y", 235.2);

$data = array(
    'a' => 0,
    'b' => 0,
    'c' => 0,
    'd' => 0
    );

$t->DefineVar($data);

// define again
$r = $t->GetVar();
echo "\n";
echo "define again:\n";
var_export($r);
echo "\n";

// clear again
$t->ClearVar();

$r = $t->GetVar();
echo "\n";
echo "clear again:\n";
var_export($r);
echo "\n";


// redefine
$t->DefineVar("H", 923.1);
$t->DefineVar("Y", 235.2);

$data = array(
    'a' => 0,
    'b' => 0,
    'c' => 0,
    'd' => 0
    );

$t->DefineVar($data);

// redefine
$r = $t->GetVar();
echo "\n";
echo "redefine:\n";
var_export($r);
echo "\n";

// compute
$t->SetExpr("a=H+Y,b=H-Y,c=H*Y,d=H/Y");

$r = $t->Eval();

echo "\n";
var_export($r);
echo "\n";

$n = 0;
$t->Eval($n);

echo "\n";
echo "after compute:\n";
var_export($t->GetVar());
echo "\n";

$t->ClearVar();

// clear
echo "\n";
echo "clear:\n";
var_export($t->GetVar());
echo "\n";

?>
--EXPECT--

first define:
array (
  'H' => 923.1,
  'Y' => 235.2,
)

after clear:
array (
)

define again:
array (
  'H' => 923.1,
  'Y' => 235.2,
  'a' => 0,
  'b' => 0,
  'c' => 0,
  'd' => 0,
)

clear again:
array (
)

redefine:
array (
  'H' => 923.1,
  'Y' => 235.2,
  'a' => 0,
  'b' => 0,
  'c' => 0,
  'd' => 0,
)

3.9247448979592

after compute:
array (
  'H' => 923.1,
  'Y' => 235.2,
  'a' => 1158.3,
  'b' => 687.9,
  'c' => 217113.12,
  'd' => 3.9247448979592,
)

clear:
array (
)
