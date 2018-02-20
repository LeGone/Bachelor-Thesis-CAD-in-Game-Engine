using EarClipperLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.SolverFoundation.Services;

namespace EarClipper
{
    class Program
    {
        static void Main(string[] args)
        {
            Helper TheHelper = new Helper();
            TheHelper.AddVertex(-1.282, 15.245, -0.080);
            TheHelper.AddVertex(-8.292, 15.245, 5.406);
            TheHelper.AddVertex(-8.292, 15.346, 5.406);
            TheHelper.AddVertex(-1.282, 15.346, -0.080);

            TheHelper.SetNormal(-0.788, 0.000, 0.616);

            List<short> Indices = TheHelper.TriangulateAndReturnIndices();

            // specify polygon points in CCW order
            List<Vector3m> Points = new List<Vector3m>()
            {
                new Vector3m(-1.282, 15.245, -0.080),
                new Vector3m(-8.292, 15.245, 5.406),
                new Vector3m(-8.292, 15.346, 5.406),
                new Vector3m(-1.282, 15.346, -0.080)
            };

            EarClipping earClipping = new EarClipping();
            earClipping.SetPoints(Points, null, new Vector3m(-0.788, 0.000, 0.616));
            earClipping.Triangulate();
            var Result = earClipping.Result;

            Indices = new List<int>();
            for (int Index = 0; Index < Result.Count; Index++)
            {
                Vector3m Vertex = Result[Index];
                for (int IndexPoint = 0; IndexPoint < Points.Count; IndexPoint++)
                {
                    if (Vertex.Equals(Points[IndexPoint]))
                    {
                        Indices.Add(IndexPoint);
                    }
                }
            }
        }
    }
}
