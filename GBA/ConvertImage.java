import javax.imageio.ImageIO;
import java.awt.image.*;
import java.util.function.Function;

public class ConvertImage {
    private static final int WIDTH = 240;
    private static final int HEIGHT = 160;

    public static void main(String[] args) throws Exception {
        String constantName = args[0];
        BufferedImage before = ImageIO.read(System.in);
        BufferedImage after = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_USHORT_555_RGB);
        after.createGraphics().drawImage(before, 0, 0, null);
        short[] buffer = ((DataBufferUShort) after.getRaster().getDataBuffer()).getData();
        Function<Short, Short> convert = ConvertImage::convert;
        Function<Short, String> stringify = convert.andThen("%04x"::formatted);
        System.out.printf("const unsigned int %s[240 * 160 / 2] = {\n", constantName);
        for (int y = 0; y < HEIGHT; y++) {
            System.out.print("    ");
            for (int x = 0; x < WIDTH - 1; x++) {
                int offset = x % 2 == 0 ? 1 : -1;
                if (x % 2 == 0) System.out.print("0x");
                System.out.print(stringify.apply(buffer[y * WIDTH + x + offset]));
                if (x % 2 == 1) System.out.print(", ");
            }
            System.out.printf("%s,\n", stringify.apply(buffer[y * WIDTH + WIDTH - 1]));
        }
        System.out.println("};");
    }

    public static Short convert(Short before) {
        return (short) (((before & 0b00000_00000_11111) << 10)
                      | ((before & 0b11111_00000_00000) >> 10)
                      | ((before & 0b00000_11111_00000)));
    }
}
