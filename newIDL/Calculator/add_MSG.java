//package org.javaClient.Calculator;
public class add_MSG {
    String funcname;
    int ret;
    add_PARAMS params;
    public add_MSG() {}
    public add_MSG(String funcname, add_PARAMS params) {
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
    public void setParams(add_PARAMS params) {
        this.params = params;
    }
    public add_PARAMS getParams() {
        return params;
    }
}
