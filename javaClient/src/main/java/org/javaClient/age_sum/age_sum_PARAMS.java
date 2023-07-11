//package org.javaClient.Calculator;
public class age_sum_PARAMS {
    user user1;
    public void setUser1(user user1) {
        this.user1 = user1;
    }
    public user getUser1() {
        return user1;
    }
    user user2;
    public void setUser2(user user2) {
        this.user2 = user2;
    }
    public user getUser2() {
        return user2;
    }
    public age_sum_PARAMS(){}
    public age_sum_PARAMS(user user1, user user2) {
        this.user1 = user1;
        this.user2 = user2;
    }
}
