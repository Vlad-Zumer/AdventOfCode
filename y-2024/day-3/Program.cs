// dotnet new console --use-program-main
// dotnet run

using System.Text.RegularExpressions;
namespace AOC2024.Day3;

class Program
{
    static void p1(string input)
    {
        string pattern = @"mul\((?<n1>[0-9]{1,3}),(?<n2>[0-9]{1,3})\)";
        RegexOptions options = RegexOptions.Multiline;

        ulong sum = 0;
        foreach (Match m in Regex.Matches(input, pattern, options))
        {
            int n1 = int.Parse(m.Groups["n1"].ToString());
            int n2 = int.Parse(m.Groups["n2"].ToString());
            sum += (ulong)(n1 * n2);
        }

        Console.WriteLine($"P1= {sum}");
    }

    static void p2(string input)
    {
        string pattern = @"(do\(\))|(don't\(\))|(mul\((?<n1>[0-9]{1,3}),(?<n2>[0-9]{1,3})\))";
        RegexOptions options = RegexOptions.Multiline;

        ulong sum = 0;
        bool enabled = true;
        // this only works if the matches get returned in the same order they appear in the text
        foreach (Match m in Regex.Matches(input, pattern, options))
        {
            // Console.WriteLine($"{m.Value}");
            if (m.Value == "do()")
            {
                enabled = true;
            }
            else if (m.Value == "don't()")
            {
                enabled = false;
            }
            else if (enabled)
            {
                int n1 = int.Parse(m.Groups["n1"].ToString());
                int n2 = int.Parse(m.Groups["n2"].ToString());
                sum += (ulong)(n1 * n2);
            }
        }

        Console.WriteLine($"P2= {sum}");
    }

    static void Main(string[] args)
    {
        // string fileName = @"test.txt";
        string fileName = @"input.txt";
        string text = File.ReadAllText(fileName);
        p1(text);
        p2(text);
    }
}
