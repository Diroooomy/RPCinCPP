public class user {
    String name;
    public void setName(String name) {
        this.name = name;
    }
    public String getName() {
        return name;
    }
    int age;
    public void setAge(int age) {
        this.age = age;
    }
    public int getAge() {
        return age;
    }
    public user(){}
    public user(String name, int age) {
        this.name = name;
        this.age = age;
    }
}
