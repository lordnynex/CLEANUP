<html>
	<head>
		<title>Sync App</title>
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
        <h1><a href="/index.php">Home</a>::{$site.site_name}</h1>
    </div>

    <div style="clear: both">
    </div>

    <div id="content-container">
        <h2>&nbsp;Sync Output&nbsp;</h2>
<pre><code>
{$ret}
</code></pre>
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
	<pre>{$sites|print_r}</pre>
{/if}

</body>
</html>
