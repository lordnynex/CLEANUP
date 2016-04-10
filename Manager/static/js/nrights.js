/**
 * @author nrights.ngx
 */


function dialogAjaxDoneEx(json){
      //添加用户时，使用这个回调，显示的对话框消失。
      alertMsg.warn(json.message); 
      if (json.statusCode == DWZ.statusCode.ok){
            if (json.navTabId){
                  navTab.reload(json.forwardUrl, {navTabId: json.navTabId});
            } else if (json.rel) {
                  var $pagerForm = $("#pagerForm", navTab.getCurrentPanel());
                  var args = $pagerForm.size()>0 ? $pagerForm.serializeArray() : {}
                  navTabPageBreak(args, json.rel);
            }
            if ("closeCurrent" == json.callbackType) {
                  $.pdialog.closeCurrent();
            }
      }
}