package arch.sm213.machine.student;

import machine.AbstractMainMemory;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class MainMemoryTest {

    private MainMemory testMainMemory;

    @BeforeEach
    void runBefore() {
        testMainMemory = new MainMemory(32);
    }

    @Test
    public void isAccessAlignedTest() {
        assertTrue(testMainMemory.isAccessAligned(0,4));
        assertFalse(testMainMemory.isAccessAligned(1,4));
        assertTrue(testMainMemory.isAccessAligned(4,2));
        assertFalse(testMainMemory.isAccessAligned(1,3));
        assertTrue(testMainMemory.isAccessAligned(80,4));
        assertFalse(testMainMemory.isAccessAligned(33,4));
    }

    @Test
    public void bytesToIntegerTest() {
        assertEquals(0, byteToIntegerModified(testMainMemory, 0,0,0,0));
        assertEquals(10, byteToIntegerModified(testMainMemory, 0, 0x00,0x00, 0x0A));
        assertEquals(128, byteToIntegerModified(testMainMemory,0x00,0x00,0x00,0x80));
        assertEquals(178, byteToIntegerModified(testMainMemory, 0x00, 0x00,0x00, 0xB2));
        assertEquals(-1, byteToIntegerModified(testMainMemory, 0xFF,0xFF, 0xFF, 0xFF));
        assertEquals(-2147483648, byteToIntegerModified(testMainMemory, 0x80,0x00,0x00,0x00));
        assertEquals(2147483647, byteToIntegerModified(testMainMemory, 0x7F,0xFF, 0xFF,0xFF));
    }

    @Test
    public void integerToBytesTest() {
        byte[] zero = {0,0,0,0};
        byte[] test = testMainMemory.integerToBytes(0);
        for (int i = 0; i < 4; i++) {
            assertEquals(zero[i], test[i]);
        }

        byte[] mem1 = {0,0,0,(byte) 0x80};
        byte[] test1 = testMainMemory.integerToBytes(128);
        for (int i = 0; i < 4; i++) {
            assertEquals(mem1[i], test1[i]);
        }

        byte[] mem2 = {0,0,0, (byte) 0xff};
        byte[] test2 = testMainMemory.integerToBytes(255);
        for (int i = 0; i < 4; i++) {
            assertEquals(mem2[i], test2[i]);
        }

        byte[] mem3 = {(byte) 0x7f,(byte) 0xff,(byte) 0xff,(byte) 0xff};
        byte[] test3 = testMainMemory.integerToBytes(Integer.MAX_VALUE);
        for (int i = 0; i < 4; i++) {
            assertEquals(mem3[i], test3[i]);
        }

        byte[] mem4 = {(byte) 0x80,0,0,0};
        byte[] test4 = testMainMemory.integerToBytes(Integer.MIN_VALUE);
        for (int i = 0; i < 4; i++) {
            assertEquals(mem4[i], test4[i]);
        }
    }

    @Test
    public void mainMemorySetGetTest() throws AbstractMainMemory.InvalidAddressException {
        byte[] zero = {0,0,0,0};
        byte[] one = {1,2,3,4};
        byte[] longCase = {1,2,3,4,5,6,7,8};

        testMainMemory.set(0,one);
        byte[] test = testMainMemory.get(0,4);
        for (int i = 0; i < 4; i++) {
            assertEquals(one[i], test[i]);
        }

        testMainMemory.set(8,longCase);
        byte[] test1 = testMainMemory.get(8,8);
        for (int i = 0; i < 4; i++) {
            assertEquals(longCase[i], test1[i]);
        }

        try {
            testMainMemory.set(0, zero);
            testMainMemory.get(0,4);
        } catch (AbstractMainMemory.InvalidAddressException exception) {
            fail("No exception should be caught");
        }

        try {
            testMainMemory.set(0, one);
            testMainMemory.get(0,4);
        } catch (AbstractMainMemory.InvalidAddressException e) {
            fail("No exception should be caught");
        }

        try {
            testMainMemory.set(4, one);
            testMainMemory.get(4,4);
        } catch (AbstractMainMemory.InvalidAddressException e) {
            fail("No exception should be caught");
        }
    }

    @Test
    public void setGetOutOfBoundsTest() throws AbstractMainMemory.InvalidAddressException {
        byte[] one = {1,2,3,4};

        try {
            testMainMemory.set(-1, one);
            fail("Exception not thrown");
        } catch (AbstractMainMemory.InvalidAddressException e) {

        }

        try {
            testMainMemory.set(33, one);
            fail("Exception not thrown");
        } catch (AbstractMainMemory.InvalidAddressException e) {

        }
    }

    private int byteToIntegerModified(MainMemory m, int one, int two, int three, int four) {
        return m.bytesToInteger((byte)one, (byte)two, (byte) three, (byte) four);
    }
}
