namespace AOT.Day4;

using System.Text;

/*
XMAS  -hFlip->  SAMX    | X             S
                        | M   -vFlip->  A
                        | A             M
                        | S             X
                        |
(0,0,X) -----> (3,0,X)  | (0,0,X) -----> (0,3,X)
(1,0,M)        (2,0,M)  | (0,1,M)        (0,2,M)
(2,0,A)        (1,0,A)  | (0,2,A)        (0,1,A)
(3,0,S)        (0,0,S)  | (0,3,S)        (0,0,S)
--------------------------------------------------------------------------
X...            ...S    | X...             S...   | X...            ...X
.M..   -vFlip-> ..A.    | .M..   -vhFlip-> .A..   | .M..   -hFlip-> ..M.
..A.            .M..    | ..A.             ..M.   | ..A.            .A..
...S            X...    | ...S             ...X   | ...S            S...
                        |                         |
(0,0,X) -----> (0,3,X)  | (0,0,X) -----> (3,3,X)  | (0,0,X) -----> (3,0,X)
(1,1,M)        (1,2,M)  | (1,1,M)        (2,2,M)  | (1,1,M)        (2,1,M)
(2,2,A)        (2,1,A)  | (2,2,A)        (1,1,A)  | (2,2,A)        (1,2,A)
(3,3,S)        (3,0,S)  | (3,3,S)        (0,0,S)  | (3,3,S)        (0,3,S)
--------------------------------------------------------------------------
M.m              m.M    | M.m              s.S    |
.A.    -hFlip->  .A.    | .A.    -vFlip->  .A.    |
s.S              S.s    | s.S              M.m    |
                        |                         |
(0,0,M) -----> (2,0,M)  | (0,0,M) -----> (0,2,M)  |
(2,0,m)        (0,0,m)  | (2,0,m)        (2,2,m)  |
(1,1,A)        (1,1,A)  | (1,1,A)        (1,1,A)  |
(2,2,S)        (0,2,S)  | (2,2,S)        (2,0,S)  |
(0,2,s)        (2,2,s)  | (0,2,s)        (0,0,s)  |
--------------------------------------------------------------------------
M.S              S.M    | M.S              m.s    |
.A.    -hFlip->  .A.    | .A.    -vFlip->  .A.    |
m.s              s.m    | m.s              M.S    |
                        |                         |
(0,0,M) -----> (2,0,M)  | (0,0,M) -----> (0,2,M)  |
(0,2,m)        (2,2,m)  | (0,2,m)        (0,0,m)  |
(1,1,A)        (1,1,A)  | (1,1,A)        (1,1,A)  |
(2,0,S)        (0,0,S)  | (2,0,S)        (2,2,S)  |
(2,2,s)        (0,2,s)  | (2,2,s)        (2,0,s)  |


*/

using SlimKernel = Program.SlimKernelItem[];

static class Extensions
{
    public static string Dump(this SlimKernel kern)
    {
        StringBuilder builder = new();
        builder.Append("Kernel:");
        foreach (var item in kern)
        {
            builder.Append($"{item.ToString()}");
        }

        return builder.ToString();
    }

    public static (int xSpan, int ySpan) getSpan(this SlimKernel kern)
    {
        int mx = 0, my = 0;
        foreach (var item in kern)
        {
            if (mx < item.x)
            {
                mx = item.x;
            }

            if (my < item.y)
            {
                my = item.y;
            }
        }

        return (mx, my);
    }

}

class Program
{
    public struct SlimKernelItem
    {
        public int x, y;
        public char c;

        public override string ToString()
        {
            return $"({x},{y},{c})";
        }
    }

    static SlimKernel vFlipKernel(SlimKernel kernel)
    {
        (_, int rowSpan) = kernel.getSpan();
        List<SlimKernelItem> ret = [];
        foreach (SlimKernelItem kernItem in kernel)
        {
            ret.Add(new SlimKernelItem() { x = kernItem.x, y = rowSpan - kernItem.y, c = kernItem.c });
        }
        return ret.ToArray();
    }

    static SlimKernel hFlipKernel(SlimKernel kernel)
    {
        (int colSpan, _) = kernel.getSpan();
        List<SlimKernelItem> ret = [];
        foreach (SlimKernelItem kernItem in kernel)
        {
            ret.Add(new SlimKernelItem() { x = colSpan - kernItem.x, y = kernItem.y, c = kernItem.c });
        }
        return ret.ToArray();
    }

    static void DebugSolution(string[] lines, List<(int, int, SlimKernel)> finds)
    {
        // horrible horrible code
        StringBuilder debugBuilder = new();
        for (int lineIndex = 0; lineIndex < lines.Length; lineIndex++)
        {
            int MaxCols = lines[lineIndex].Length;
            debugBuilder.Append($"{new string('.', MaxCols)}\n");
        }

        string[] debugMat = debugBuilder.ToString().Split('\n');

        foreach ((int startX, int startY, SlimKernel kernel) in finds)
        {
            foreach (SlimKernelItem item in kernel)
            {
                var chArr = debugMat[startY + item.y].ToCharArray();
                chArr[startX + item.x] = item.c;
                debugMat[startY + item.y] = new string(chArr);
            }
        }

        foreach (var line in debugMat)
        {
            Console.WriteLine("{0}", line);
        }
    }

    static ulong Solve(string[] lines, SlimKernel[] kernels, bool debug = false)
    {
        List<(int, int, SlimKernel)> finds = [];
        ulong sum = 0;

        int linesSize = lines.Length;
        for (int lineIndex = 0; lineIndex < linesSize; lineIndex++)
        {
            int colsSize = lines[lineIndex].Length;
            for (int colIndex = 0; colIndex < colsSize; colIndex++)
            {
                foreach (var kernel in kernels)
                {
                    (int colSpan, int rowSpan) = kernel.getSpan();

                    if (colIndex + colSpan >= colsSize || lineIndex + rowSpan >= linesSize)
                    {
                        continue;
                    }

                    bool ok = true;
                    foreach (var item in kernel)
                    {
                        if (item.c != lines[item.y + lineIndex][item.x + colIndex])
                        {
                            ok = false;
                            break;
                        }
                    }

                    if (ok)
                    {
                        finds.Add((colIndex, lineIndex, (SlimKernel)kernel.Clone()));
                        sum++;
                    }
                }
            }
        }

        if (debug)
        {
            Console.WriteLine("[P1] DEBUG START:\n");
            DebugSolution(lines, finds);
            Console.WriteLine("[P1] DEBUG END\n");
        }

        return sum;
    }

    static ulong p1(string[] lines, bool debug = false)
    {
        SlimKernel horizontal = [
            new SlimKernelItem(){x = 0, y = 0, c = 'X'},
            new SlimKernelItem(){x = 1, y = 0, c = 'M'},
            new SlimKernelItem(){x = 2, y = 0, c = 'A'},
            new SlimKernelItem(){x = 3, y = 0, c = 'S'}
        ];

        SlimKernel vertical = [
            new SlimKernelItem(){x = 0, y = 0, c = 'X'},
            new SlimKernelItem(){x = 0, y = 1, c = 'M'},
            new SlimKernelItem(){x = 0, y = 2, c = 'A'},
            new SlimKernelItem(){x = 0, y = 3, c = 'S'}
        ];

        SlimKernel diag = [
            new SlimKernelItem(){x = 0, y = 0, c = 'X'},
            new SlimKernelItem(){x = 1, y = 1, c = 'M'},
            new SlimKernelItem(){x = 2, y = 2, c = 'A'},
            new SlimKernelItem(){x = 3, y = 3, c = 'S'}
        ];

        SlimKernel[] kernels = [ horizontal,
                                 hFlipKernel(horizontal),
                                 vertical,
                                 vFlipKernel(vertical),
                                 diag,
                                 vFlipKernel(diag),
                                 hFlipKernel(diag),
                                 vFlipKernel(hFlipKernel(diag)) ];

        return Solve(lines, kernels, debug);
    }

    static ulong p2(string[] lines, bool debug = false)
    {
        // only vFlip matters
        SlimKernel mLine = [
            new SlimKernelItem(){x = 0, y = 0, c = 'M'},
            new SlimKernelItem(){x = 2, y = 0, c = 'M'},
            new SlimKernelItem(){x = 1, y = 1, c = 'A'},
            new SlimKernelItem(){x = 2, y = 2, c = 'S'},
            new SlimKernelItem(){x = 0, y = 2, c = 'S'},
        ];

        // only hFlip matters
        SlimKernel mCol = [
            new SlimKernelItem(){x = 0, y = 0, c = 'M'},
            new SlimKernelItem(){x = 0, y = 2, c = 'M'},
            new SlimKernelItem(){x = 1, y = 1, c = 'A'},
            new SlimKernelItem(){x = 2, y = 0, c = 'S'},
            new SlimKernelItem(){x = 2, y = 2, c = 'S'},
        ];

        SlimKernel[] kernels = [ mLine,
                                 vFlipKernel(mLine),
                                 mCol,
                                 hFlipKernel(mCol) ];

        return Solve(lines, kernels, debug);
    }

    static void Main(string[] args)
    {
        // string fileName = "test.txt";
        string fileName = "input.txt";

        string[] lines = File.ReadAllLines(fileName);
        Console.WriteLine("P1 = {0}", p1(lines));
        Console.WriteLine("P2 = {0}", p2(lines));
        Console.WriteLine("DONE!");
    }
}
