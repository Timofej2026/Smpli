using System;
using System.IO;
using System.Text.RegularExpressions;
using System.Collections.Generic;

public class SimpCompiler
{
    private static List<string> validTypes = new List<string> {
        "3Dmodel", "2DSprite", "textDocument", "script", "pythonScript",
        "3Dprefab", "2Dprefab", "3Dscene", "2Dscene", "picturePNG",
        "pictureJPG", "videoMP4", "videoMOV", "musicMP3", "musicOGG"
    };
    private static string currentMsg = "";
    private static string pressedKey = null;
    private static int[] xyz = new int;
    private static string dataType = null;
    private static string hintText = "";
    private static Dictionary<string, object> variables = new Dictionary<string, object>();

    public static void CompileAndRun(string filename)
    {
        if (!File.Exists(filename) || !filename.EndsWith(".simp")) return;
        string[] lines = File.ReadAllLines(filename);

        foreach (string line in lines)
        {
            string tLine = line.Trim();
            if (string.IsNullOrEmpty(tLine) || tLine == "{" || tLine == "}" || tLine == "start" || tLine == "data") continue;
            if (tLine == "stop") Environment.Exit(0);
            if (tLine.StartsWith("scriptTag(") || tLine.StartsWith("use(")) continue;

            if (tLine.StartsWith("addDataType(") && tLine.EndsWith(")"))
            {
                validTypes.Add(tLine.Substring(12, tLine.Length - 13).Trim());
                continue;
            }
            if (tLine.StartsWith("dataType(") && tLine.EndsWith(")"))
            {
                string dt = tLine.Substring(9, tLine.Length - 10).Trim();
                if (validTypes.Contains(dt)) dataType = dt;
                continue;
            }
            if (tLine.StartsWith("print(") && tLine.EndsWith(")"))
            {
                string val = tLine.Substring(6, tLine.Length - 7).Trim().Replace("\"", "");
                Console.WriteLine(variables.ContainsKey(val) ? variables[val] : val);
                continue;
            }
            if (tLine.StartsWith("alert(") && tLine.EndsWith(")"))
            {
                Console.WriteLine(tLine.Substring(6, tLine.Length - 7).Trim().Replace("\"", ""));
                continue;
            }
            if (tLine == "alertSound()")
            {
                Console.Beep();
                continue;
            }
            if (tLine.StartsWith("ccor(") && tLine.EndsWith(")"))
            {
                string[] coords = tLine.Substring(5, tLine.Length - 6).Split(',');
                if (coords.Length == 3) xyz = new int[] { int.Parse(coords.Trim()), int.Parse(coords.Trim()), int.Parse(coords.Trim()) };
                continue;
            }
            if (tLine.Contains("=") && tLine.Contains("(") && tLine.EndsWith(")"))
            {
                var match = Regex.Match(tLine, @"([a-zA-Z0-9_()]+)\s*=\s*\((.*)\)");
                if (match.Success)
                {
                    string name = match.Groups[1].Value.Replace("()", "").Trim();
                    string expr = match.Groups[2].Value.Trim().Replace("\"", "");
                    variables[name] = expr;
                }
                continue;
            }
        }
    }

    public static void Main(string[] args)
    {
        if (args.Length > 0) CompileAndRun(args[0]);
    }
}
