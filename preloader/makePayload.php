<?php
/**
 * Copyright(C) Stefano Moioli <smxdev4@gmail.com>
 */
function align($value, $size){
	$mask = $size-1;
	return ($value + $mask) & ~($mask);
}

function pathConcat(...$parts){
	return implode(DIRECTORY_SEPARATOR, $parts);
}

if($argc < 5){
	fwrite(STDERR, "Usage: {$argv[0]} [preloader.bin] [payload.bin] [LOADADDR] [output.bin]" . PHP_EOL);
	return 1;
}
list(
	$preloaderFile,
	$payloadFile,
	$loadaddr,
	$outputFile
) = array_slice($argv, 1);
$loadAddr = intval($loadaddr, 16);

$hOut = fopen($outputFile, "wb");
if(!is_resource($hOut)){
	fwrite(STDERR, "Cannot open output file '{$outputFile}' for writing" . PHP_EOL);
	exit(1);
}

$hLdr = fopen($preloaderFile, "rb");
if(!is_resource($hLdr)){
	fwrite(STDERR, "Cannot open preloader file '{$preloaderFile}' for reading" . PHP_EOL);
	fclose($hOut);
	exit(1);
}

$hPayload = fopen($payloadFile, "rb");
if(!is_resource($hPayload)){
	fwrite(STDERR, "Cannot open payload file '{$payloadFile}' for reading" . PHP_EOL);
	fclose($hLdr);
	fclose($hOut);
	exit(1);
}

// copy the loader
stream_copy_to_stream($hLdr, $hOut, filesize($preloaderFile) - 8);

$magic1 = unpack("V", fread($hLdr, 4))[1];
$magic2 = unpack("V", fread($hLdr, 4))[1];
fclose($hLdr);

if($magic1 !== 0xDEADBEE0 || $magic2 !== 0xDEADBEE1){
	fwrite(STDERR, ".loadArgs section not found" . PHP_EOL);
	fclose($hOut);
	exit(1);
}

// calculate payload size in words
$fileSize = filesize($payloadFile);
$fileSizeAligned = align($fileSize, 4);
$numWords = $fileSizeAligned / 4;

// write loader arguments
print("Params: @0x" . dechex(ftell($hOut)) . PHP_EOL);
fwrite($hOut, pack("V", $loadAddr));
fwrite($hOut, pack("V", $numWords));

// write payload
stream_copy_to_stream($hPayload, $hOut);
// write padding, if any
str_repeat("\0", $fileSizeAligned - $fileSize);

fclose($hPayload);
fclose($hOut);
