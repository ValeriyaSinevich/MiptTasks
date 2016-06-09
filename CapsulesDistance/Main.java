public class Main {
    public static double distance(double ax0,
                                  double ay0,
                                  double bx0,
                                  double by0,
                                  double r0,
                                  double ax1,
                                  double ay1,
                                  double bx1,
                                  double by1,
                                  double r1) {
        Point start1 = new Point();
        start1.setX(ax0);
        start1.setY(ay0);

        Point start2 = new Point();
        start2.setX(ax1);
        start2.setY(ay1);

        Point end1 = new Point();
        end1.setX(bx0);
        end1.setY(by0);

        Point end2 = new Point();
        end2.setX(bx1);
        end2.setY(by1);


        double distanceBetweenAxes = Geometry.distanceBetweenIntervals(start1,
                start2, end1, end2);
        return Math.max(distanceBetweenAxes - r0 - r1, 0);
    }


    public static void main(String[] args) {
        double ax0 = Double.parseDouble(args[0]);
        double ay0 = Double.parseDouble(args[1]);
        double bx0 = Double.parseDouble(args[2]);
        double by0 = Double.parseDouble(args[3]);
        double r0 = Double.parseDouble(args[4]);
        double ax1 = Double.parseDouble(args[5]);
        double ay1 = Double.parseDouble(args[6]);
        double bx1 = Double.parseDouble(args[7]);
        double by1 = Double.parseDouble(args[8]);
        double r1 = Double.parseDouble(args[9]);
        System.out.println(distance(ax0, ay0, bx0, by0, r0, ax1, ay1, bx1, by1, r1));
    }
}
