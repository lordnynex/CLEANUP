<?php
/*
*   Copyright 2012 Brandon Beveridge
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/

require("SM.php"); 
require("Sync.php");

$config = array(
	'dbhost' => '*********',
	'dbname' => '*********',
	'dbuser' => '*********',
	'dbpass' => '*********',


);

$sync = new Sync();

$smarty = SM::getInstance();

$smarty->setTemplateDir(dirname(__FILE__)."/templates/");
$smarty->setCompileDir(dirname(__FILE__)."/lib/templates_c");
$smarty->setCacheDir(dirname(__FILE__)."/lib/cache");
$smarty->setConfigDir(dirname(__FILE__)."/lib/confi");

?>
