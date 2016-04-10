#include <nchan_module.h>

void nchan_exit_notice_about_remaining_things(char *thing, char *where, ngx_int_t num) {
  if(num > 0) {
    ngx_log_error(NGX_LOG_NOTICE, ngx_cycle->log, 0, "nchan: %i %s%s remain%s %sat exit", num, thing, num == 1 ? "" : "s", num == 1 ? "s" : "", where == NULL ? "" : where);
  }
}
