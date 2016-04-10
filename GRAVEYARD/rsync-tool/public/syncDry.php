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
require(dirname(__FILE__)."/../private/config.php");

$sync_id;

if(!isset($_GET['sync_id']) || empty($_GET['sync_id'])) {
	echo "No sync ID specified.";
	die;
} else {
	$sync_id = (int)$_GET['sync_id'];
}

$smarty->assign('site', $sync->loadSite($sync_id));
$smarty->assign('filelist', $sync->pushDry($sync_id));

$smarty->display('syncDry.tpl');

?>
