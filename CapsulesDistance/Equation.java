public class Equation {
    private double a;
    private double b;
    private double c;

    public Equation(Point vect, Point start) {
        this.a = vect.getY();
        this.b = -vect.getX();
        this.c =  -(start.getX() * this.a  + start.getY() * this.b);
    }

    public double getA() {
        return this.a;
    }

    public double getB() {
        return this.b;
    }

    public double getC() {
        return this.c;
    }

    public void setA(double a) {
        this.a = a;
    }

    public void setB(double b) {
        this.b = b;
    }

    public void setC(double c) {
        this.c = c;
    }
}
