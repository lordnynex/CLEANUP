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

//echo "<pre>"; print_r($_POST); die;

$files = $_POST['toPush'];
$sync_id = (int)$_POST['sync_id'];

$ret = $sync->pushWet($sync_id, $files);
$sync->updateSyncTime($sync_id);

$smarty->assign('site', $sync->loadSite($sync_id));
$smarty->assign('ret', implode("\n", $ret));
$smarty->display('syncWet.tpl');

?>
