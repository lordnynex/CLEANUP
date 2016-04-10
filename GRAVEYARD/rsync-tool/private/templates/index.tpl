<html>
	<head>
		<title>Rsync App</title>
		<link href="style.css" rel="stylesheet" type="text/css">
	</head>

<body>

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
        <h1>Site Sync</h1>
    </div>

    <div style="clear: both">
    </div>

    <div id="content-container">
        <h2>&nbsp;Sites Available&nbsp;</h2>

<table class="noborder" FRAME=BOX cellpadding=1 cellspacing=1 width="100%">
	<thead>
		<tr>
			<td>Site</td>
			<td>Last Sync</td>
		</tr>
	</thead>
	<tbody>
{foreach $sites as $site}
	{cycle values='even,odd' assign="class"}
	<tr class="{$class}" style="cursor:pointer;" onMouseOver="this.className='highlight'" onMouseOut="this.className='{$class}'" onClick="window.location='/syncDry.php?sync_id={$site.sync_id}'">
	        <td>{$site.site_name}</td>
		<td>{$site.last_sync}</td>
	</tr>
{/foreach}
	</tbody>
</table>

    </div>

    <div style="clear: both">
    </div>

    <div id="footer">
        <h1>&nbsp;</h1>
    </div>
</div>

{if isset($smarty.get.debug) && $smarty.get.debug eq 1}
	<br><br><br>
	<hr>
	<h1>Debug</h1>
	<pre>{$sites|print_r}</pre>
{/if}

</body>
</html>
