<?php
$hostname = $_SERVER['HTTP_HOST'];
$path = dirname($_SERVER['PHP_SELF']);
if (!isset($_SESSION['success'])) $_SESSION['success'] = "";
if (!isset($_SESSION['warning'])) $_SESSION['warning'] = "";
if (!isset($_SESSION['error'])) $_SESSION['error'] = "";
// ______________________________________________
function htb_get_path($idx) {
	global $htbconf;
	$prefix = $htbconf['paths/prefix'];
	if (substr($prefix, -1, 1) != '/') {
		$htbconf['paths/prefix'].= "/";
	}
	return (substr($htbconf['paths/' . $idx], 0, 1) == '/') ? $htbconf['paths/' . $idx] : $htbconf['paths/prefix'] . $htbconf['paths/' . $idx];
}
// ______________________________________________
function htb_load_page($page) {
	global $http;
	$pagepath = htb_get_path('pages');
	if (!file_exists($pagepath . $page . '.page')) {
		$_SESSION['error'] = 'Invalid page! Please contact your administrator!';
		htb_redirect(htb_getbaseurl());
	}
	include_once($pagepath . $page . '.page');
}
// ______________________________________________
function htb_redirect($url) {
	if (substr($url, -1, 1) == '/') $url.= "index.php";
	header("Location: " . $url);
}
// ______________________________________________
function htb_pageurl($page, $getvars = array(), $pathinfo = "", $fragment = "") {
		global $http;
	$q = "?";
	$url = htb_getbaseurl();
	$url.= "index.php";
	if ($pathinfo <> "") {
		if (substr($pathinfo, 0, 1) != "/") $pathinfo = "/" . $pathinfo;
		$url.= $pathinfo;
	}
	if ($page <> "") {
		$url.= $q . "page=" . $page;
		$q = "&";
	}
	while (list($k, $v) = each($getvars)) {
		$url.= $q . urlencode($k) . "=" . urlencode($v);
		$q = "&";
	}
	if (SID != "" && !(substr($pathinfo, 0, 6) == "/page=")) {
		$url.= $q . strip_tags(SID);
		$q = "&";
	} else if ($_COOKIE[session_name() ] == "" && !(substr($pathinfo, 0, 6) == "/page=")) {
		$url.= $q . strip_tags(session_name()) . "=" . session_id();
		$q = "&";
	}
	if ($fragment != "" && substr($fragment, 0, 1) != "#") $fragment = "#" . $fragment;
	$url.= $fragment;
	return $url;
}
// ______________________________________________
function htb_getpathinfo() {
	global $_SERVER;
	$pathInfoArray = array();
	if (isset($_SERVER['PATH_INFO'])) {
		$ps = explode("&", urldecode(substr($_SERVER['PATH_INFO'], 1)));
		while (list(, $pi) = each($ps)) {
			$k = explode("=", $pi);
			$pathInfoArray = array_merge($pathInfoArray, array($k[0] => $k[1]));
		}
	}
	return $pathInfoArray;
}
// ______________________________________________
function htb_getbaseurl() {
	$folder = substr($_SERVER['SCRIPT_NAME'], 0, strrpos($_SERVER['SCRIPT_NAME'], "/")+1);
	return sprintf("%s://%s%s",isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] != 'off' ? 'https' : 'http',$_SERVER['HTTP_HOST'],$folder);
}
// ______________________________________________
function htb_reload_page() {
	global $_SERVER;
	htb_redirect($_SERVER['REQUEST_URI']);
}
// ______________________________________________
function errorHandler($errno, $errstr, $errfile, $errline, $errcontext) {
	switch ($errno) {
		case E_USER_WARNING:
		case E_USER_NOTICE:
		case E_NOTICE:
		case E_CORE_WARNING:
		case E_COMPILE_WARNING:
		case E_USER_ERROR:
		case E_ERROR:
		case E_PARSE:
		case E_CORE_ERROR:
		case E_COMPILE_ERROR:
		case E_WARNING:
			global $query;
			if (eregi('^(sql)$', $errstr)) {
				$MYSQL_ERRNO = mysql_errno();
				$MYSQL_ERROR = mysql_error();
				$errstr = "MySQL error: $MYSQL_ERRNO : $MYSQL_ERROR";
			} else {
				$query = NULL;
			} // if
			$errorstring = "<p>Fatal Error: $errstr ($errno).</p>\n";
			$_SESSION['error'].= $errorstring;
		break;
		default:
		break;
		}
}
?>