<?php
/* $message = "HELLO WORLD";
echo $message; */

function generateAsciiTree($height) {
    $tree = '';
    for ($i = 1; $i <= $height; $i++) {
        $spaces = str_repeat(' ', $height - $i);
        $stars = str_repeat('*', 2 * $i - 1);
        $tree .= $spaces . $stars . PHP_EOL;
    }
    return $tree;
}

$treeHeight = 5;
$asciiTree = generateAsciiTree($treeHeight);
echo $asciiTree;
?>
