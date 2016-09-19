<?php

ini_set("memory_limit", 1024 * 1024);
gc_enable();

$i = 0;
for($i = 0; $i < 100000; $i ++) {
    $t = new mu\Parser();

    if(1) {
        $t->DefineConst("H", 923);
        $t->DefineConst("Y", 235.2);

        $r = $t->GetConst();

        $data = array(
            'a' => 0,
            'b' => 0,
            'c' => 0,
            'd' => 0
        );

        $t->DefineVar($data);

        $t->SetExpr("a=H+Y,b=H-Y,c=H*Y,d=H/Y");

        $r = $t->Eval($n);

        $n = 0;
        $t->Eval($n);

        $r = $t->GetConst();

        $t->DefineVar("abc", 12.5);
        $t->DefineVar("abc", 12.5);
        $t->DefineVar("abc", 12.5);
        $t->DefineVar("abc", 12.5);
        $t->DefineVar("abc", 12.5);
    }

    if($i % 1 == 0) {
        echo "\n", memory_get_usage(), "\n";
    }

    $t = null;
}
