using System.Collections.Generic;

namespace EarClipperLib
{
    public class Helper
    {
        List<Vector3m> Vertices = new List<Vector3m>();
        Vector3m Normal;

        public void AddVertex(double X, double Y, double Z)
        {
            Vertices.Add(new Vector3m(X, Y, Z));
        }

        public void SetNormal(double X, double Y, double Z)
        {
            Normal = new Vector3m(X, Y, Z);
        }

        public List<short> TriangulateAndReturnIndices()
        {
            EarClipping EarClipper = new EarClipping();
            EarClipper.SetPoints(Vertices, null, Normal);
            EarClipper.Triangulate();
            var Result = EarClipper.Result;

            List<short> Indices = new List<short>();
            for (short Index = 0; Index < Result.Count; Index++)
            {
                Vector3m Vertex = Result[Index];
                for (short IndexPoint = 0; IndexPoint < Vertices.Count; IndexPoint++)
                {
                    if (Vertex.Equals(Vertices[IndexPoint]))
                    {
                        Indices.Add(IndexPoint);
                    }
                }
            }
            return Indices;
        }
    }
}