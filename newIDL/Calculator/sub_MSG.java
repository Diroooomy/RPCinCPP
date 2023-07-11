//package org.javaClient.Calculator;
public class sub_MSG {
    String funcname;
    int ret;
    sub_PARAMS params;
    public sub_MSG() {}
    public sub_MSG(String funcname, sub_PARAMS params) {
        this.funcname=funcname;
        this.params=params;
    }
    public void setFuncname(String funcname) {
        this.funcname = funcname;
    }
    public String getFuncname() {
        return funcname;
    }
    public void setRet(int ret) {
        this.ret = ret;
    }
    public int getRet() {
        return ret;
    }
    public void setParams(sub_PARAMS params) {
        this.params = params;
    }
    public sub_PARAMS getParams() {
        return params;
    }
}
