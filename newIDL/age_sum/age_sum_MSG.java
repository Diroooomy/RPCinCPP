//package org.javaClient.Calculator;
public class age_sum_MSG {
    String funcname;
    int ret;
    age_sum_PARAMS params;
    public age_sum_MSG() {}
    public age_sum_MSG(String funcname, age_sum_PARAMS params) {
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
    public void setParams(age_sum_PARAMS params) {
        this.params = params;
    }
    public age_sum_PARAMS getParams() {
        return params;
    }
}
