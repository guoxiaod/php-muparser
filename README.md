php-muparser
============

muparser php extension


###  Compile step
1. sudo apt-get install libmuparser-dev
2. git clone https://github.com/guoxiaod/php-muparser.git
3. cd php-muparser/php-muparser
4. phpize && ./configure && make && sudo make install

### Usage

  <?php

  use mu\Parser;

  $parser = new Parser();
  $arr = array(
    "a" => 1,
    "b" => 2,
    "c" => 3
    );
  $parser->DefineVars($arr);

  $parser->SetExpr("a=b+c,b=a+c,c=a+b");

  try {
     $n = 0;

     echo "\n====result before eval====\n";
     var_export($result);

     $result = $parser->Eval($n);

     echo "\n====result after eval====\n";
     var_export($result);
     
     echo "\n====arr====\n";
     var_export($arr);

     echo "\n====var====\n";
     var_export($parser->GetVar());
  } catch (ParserException $e) {
     echo $e, "\n"; 
  }
