<html>
        <head>
                <title>Rsync App</title>
                <link href="style.css" rel="stylesheet" type="text/css">
        </head>

<body>

<script language="JavaScript">
function toggle(source, name) {
	checkboxes = document.getElementsByName(name);
	for(var i in checkboxes)
		checkboxes[i].checked = source.checked;
}

function toggle2(source) {
	var td = source.previousSibling.previousSibling;
	var checkbox = td.firstChild;

	if(checkbox.checked == true) {
		checkbox.checked = false;
	} else {
		checkbox.checked = true;
	}
}
</script>

<style TYPE="text/css">
<!--
  tr { background-color: #DDDDDD }
  .initial { background-color: #DDDDDD; color:#000000 }
  .normal { background-color: #CCCCCC }
  .highlight { background-color: #8888FF }
  .even { background-color: #DDDDDD; }
  .odd { background-color: #CCCCCC; }
  .noborder, .noborder tr, .noborder th, .noborder td { border: none; }
-->
</style>


<div id="outer-container">
    <div id="header">
        <h1><a href="/index.php">Home</a></h1>
    </div>

    <div style="clear: both">
    </div>

    <div id="content-container">
        <h2>&nbsp;{$site.site_name}&nbsp;</h2>
<br>
Excludes:
<div style="background-color: #DDDDDD;">
{$site.site_excludes}
</div>
<br>
	<h2>&nbsp;Dry Run&nbsp;</h2>
<P>
<form action="/sync.php" method="POST">
<input type="hidden" name="sync_id" value="{$site.sync_id}">
<table class="noborder" FRAME=BOX cellpadding=1 cellspacing=1 width="100%">
	<thead>
		<tr style="background-color: #DDDDDD">
			<th>&nbsp;</th>
			<th>File/Directory</th>
		</tr>
	</thead>
	<tbody>
{if count($filelist.add) <= 0}
	<tr style="background-color: #FFFFFF;"><td colspan=2>&nbsp;</td></tr>
	<tr>
		<td colspan=2>No files to be pushed</td>
	</tr>
{else}
	{foreach $filelist.add as $file}
			{cycle values='even,odd' assign="class"}
			<tr class="{$class}" onMouseOver="this.className='highlight'" onMouseOut="this.className='{$class}'">
				<td><input class="checkbox" type="checkbox" name="toPush[]" value="{$file}"></td>
				<td onClick="toggle2(this);">{$file}</td>
			</tr>
	{/foreach}
{/if}

	</tbody>
	<tfoot>
		<tr style="background-color: #FFFFFF">
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
		<tr style="background-color: #DDDDDD">
			<td><input type="checkbox" onClick="toggle(this, 'toPush[]');" {if count($filelist.add) <= 0}DISABLED{/if}></td>
			<td onClick="toggle(this, 'toPush[]');">Select All</td>
		</tr>
	</tfoot>
</table>
<input type="submit" value="Submit" />
</form>
</P>

    </div>

    <div style="clear: both">
    </div>

<!--
    <div id="footer">
        <h1>&nbsp;</h1>
    </div>
-->
</div>

{if isset($smarty.get.debug) && $smarty.get.debug eq 1}
	<br><br><br>
	<hr>
	<h1>Debug</h1>
	<h2>File List</h2>
	<pre>{$filelist|print_r}</pre>
	<h2>Site Model</h2>
	<pre>{$site|print_r}</pre>
{/if}

</body>
</html>
