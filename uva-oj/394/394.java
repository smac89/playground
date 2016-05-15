import java.io.IOException;
import java.util.Scanner;
import java.util.HashMap;
import java.lang.NullPointerException;
import java.lang.StringBuilder;

class AMMarray
{   
    public final int C[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    public final int D;
    private static int bounds[][] = 
    {
        {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
        {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
    };
    
    public AMMarray(Scanner Reader) throws IOException
    {                   
        int baseAddr = Reader.nextInt(),
        sBytes = Reader.nextInt(), d, a = 0;
        D = Reader.nextInt();
        d = D;
        
        for (int i = 0; i < D; ++i)
        {
            bounds[i][0] = Reader.nextInt();
            bounds[i][1] = Reader.nextInt();
        }
        
        a += (C[d] = sBytes) * bounds[d - 1][0];
        while (--d > 0)
            a += (C[d] = C[1 + d] * (bounds[d][1] -
            bounds[d][0] + 1)) * bounds[d - 1][0];
        C[d] = baseAddr - a;
    }
}


class Main
{
    
    static StringBuilder outString;
    static Scanner Reader;
    
    public static void main(String args[]){
        run();
    }
    
    static void run()
    {
        outString = new StringBuilder();
        Reader = new Scanner(System.in);
        try
        {
            solve();
        }
        catch ( IOException ioe ) {}
        catch ( NullPointerException npe ) {} 
        finally
        {
            System.out.print(outString.toString());
        }
    }
    
    static void solve() throws IOException
    {
        HashMap<String, AMMarray> aMM = new HashMap();
        
        int num, ref;
        num = Reader.nextInt();
        ref = Reader.nextInt();
        
        while(num-- > 0)
            aMM.put(Reader.next(), new AMMarray(Reader));
        while(ref-- > 0)
        {
            String name = Reader.next();
            phyAddr(aMM.get(name), name);
        }
    }
    
    static void phyAddr(AMMarray arr, String name) throws IOException
    {
        int temp = Reader.nextInt(), value = temp * arr.C[1];
        outString.append(name + "[" + temp);
        for (int t = 2; t <= arr.D; ++t)
        {
            temp = Reader.nextInt();
            outString.append(", " + temp);
            value += temp * arr.C[t];
        }
        
        outString.append("] = " + (value + arr.C[0]) + "\n");
    }
}
