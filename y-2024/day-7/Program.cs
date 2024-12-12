namespace AOT.Day7
{
    using System.Collections;
    using ParsedInputItem = (ulong result, ulong[] numbers);

    static class Program
    {
        static ParsedInputItem[] ParseInput(string fileName)
        {
            List<(ulong result, ulong[] numbers)> results = [];
            string[] lines = File.ReadAllLines(fileName);
            foreach (string line in lines)
            {
                var split = line.Split(':');
                ulong result = ulong.Parse(split[0]);
                ulong[] numbers = split[1].Split(' ').Where(str => str != "").Select(str => ulong.Parse(str)).ToArray();
                results.Add((result, numbers));
            }

            return results.ToArray();
        }

        enum Op
        {
            Add,
            Mul,
            App,
        }

        static ulong ConcatNum(ulong arg1, ulong arg2)
        {
            ulong mult = 1;
            while (mult <= arg2)
            {
                mult *= 10;
            }
            return mult * arg1 + arg2;
        }

        static Func<ulong, ulong> OpFunc(this Op op, ulong arg2)
        {
            switch (op)
            {
                case Op.Add: return (arg1) => arg1 + arg2;
                case Op.Mul: return (arg1) => arg1 * arg2;
                case Op.App: return (arg1) => ConcatNum(arg1, arg2);
            }
            throw new Exception("Unrecognized operation");
        }

        static ulong ApplyOps(ulong[] nums, Op[] ops)
        {
            // 9 (+7) (*18) (+13)
            ulong first = nums.First();
            return ops.Zip(nums.Skip(1)).Select(opNum => opNum.First.OpFunc(opNum.Second)).Aggregate(first, (acc, val) => val(acc));
        }

        static Op[] GenNextOpsP1(Op[] ops)
        {
            List<Op> ret = ops.ToList();
            int lastIndex = ret.Count - 1;
            while (lastIndex >= 0 && ret[lastIndex] == Op.Mul)
            {
                lastIndex--;
            }

            if (lastIndex < 0)
            {
                return [];
            }

            ret[lastIndex] = Op.Mul;

            for (int i = lastIndex + 1; i < ops.Length; i++)
            {
                ret[i] = Op.Add;
            }

            return ret.ToArray();
        }

        static Op[] GenNextOpsP2(Op[] ops)
        {
            var getNextOp = (Op op) => op switch
            {
                Op.Add => Op.Mul,
                Op.Mul => Op.App,
                Op.App => throw new Exception("Unreachable"),
                _ => throw new NotImplementedException()
            };

            List<Op> ret = ops.ToList();
            int lastIndex = ret.Count - 1;
            while (lastIndex >= 0 && ret[lastIndex] == Op.App)
            {
                lastIndex--;
            }

            if (lastIndex < 0)
            {
                return [];
            }

            ret[lastIndex] = getNextOp(ret[lastIndex]);

            for (int i = lastIndex + 1; i < ops.Length; i++)
            {
                ret[i] = Op.Add;
            }

            return ret.ToArray();
        }

        static ulong Solve(ParsedInputItem input, Func<Op[], Op[]> genNextOps)
        {
            Op[] currOps = Enumerable.Repeat(Op.Add, input.numbers.Length - 1).ToArray();
            while (input.result != ApplyOps(input.numbers, currOps))
            {
                currOps = genNextOps(currOps);
                if (currOps.Length == 0)
                {
                    return 0;
                }

            }
            return input.result;
        }

        static ulong P1(ParsedInputItem[] input)
        {
            var sum = input.AsParallel().Select(item => Solve(item, GenNextOpsP1)).Aggregate((acc, val) => acc + val);
            return sum;
        }

        static ulong P2(ParsedInputItem[] input)
        {
            var sum = input.AsParallel().Select(item => Solve(item, GenNextOpsP2)).Aggregate((acc, val) => acc + val);
            return sum;
        }

        static void Main(string[] args)
        {
            // string fileName = "test.txt";
            string fileName = "input.txt";
            var input = ParseInput(fileName);

            Console.WriteLine("P1: {0}", P1(input));
            Console.WriteLine("P2: {0}", P2(input));
        }
    }
}
