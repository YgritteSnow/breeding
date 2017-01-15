using System.Collections.Generic;
using UnityEngine;

public class RadarMap : MonoBehaviour
{
    List<Vector3> bgMesh = new List<Vector3>();
    List<Vector3> radarMesh = new List<Vector3>();
    List<int> faces = new List<int>();

    Mesh radar;
    Mesh background;

    [SerializeField]
    MeshFilter radarFilter;

    [SerializeField]
    MeshFilter bgFilter;

    [SerializeField]
    MeshRenderer radarRenderer;

    [SerializeField]
    LineRenderer bgLinesRenderer;

    [SerializeField]
    float radius = 1.0f;

    [SerializeField]
    List<string> titles = new List<string>();

    void Awake()
    {
        radarRenderer.material.color = new Color(1, 0.5f, 0.5f, 0.5f);

        radarMesh.Add(Vector3.zero);
        bgMesh.Add(Vector3.zero);
        for (int i = 0; i < 6; i++)
        {
            float radian = 2.0f * Mathf.PI * i / 6.0f;
            float cosr = Mathf.Cos(radian);
            float sinr = Mathf.Sin(radian);
            bgMesh.Add(new Vector3(cosr, sinr, 0) * radius);
            radarMesh.Add(new Vector3(cosr, sinr, 0) * radius * 0.5f);
        }

        for (int i = 1; i <= 6; i++)
        {
            faces.Add(0);
            faces.Add((i + 1) % 7 + (i / 6));
            faces.Add(i);
        }

        radar = new Mesh();
        background = new Mesh();
        radar.vertices = radarMesh.ToArray();
        background.vertices = bgMesh.ToArray();
        radar.SetTriangles(faces, 0);
        background.SetTriangles(faces, 0);

        radarFilter.mesh = radar;
        bgFilter.mesh = background;

        Vector3[] lines = new Vector3[faces.Count];
        for (int i = 0; i < faces.Count; i++)
        {
            lines[i] = bgMesh[faces[i]];
        }
        bgLinesRenderer.numPositions = lines.Length;
        bgLinesRenderer.SetPositions(lines);
    }

    public void ResetMap(DNA dna)
    {
        for (int i = 1; i <= 6; i++)
        {
            radarMesh[i] = bgMesh[i];
        }
        radarMesh[1] *= dna.RMGene;
        radarMesh[2] *= dna.RMSpeed;
        radarMesh[3] *= dna.RMPhysic;
        radarMesh[4] *= dna.RMMagic;
        radarMesh[5] *= dna.RMOffense;
        radarMesh[6] *= dna.RMDefense;
        radar.vertices = radarMesh.ToArray();
    }

    void OnGUI()
    {
        for (int i = 1; i <= 6; i++)
        {
            Vector3 t = bgMesh[i] * 1.5f;
            Vector3 p = transform.position;
            t.y = -t.y;
            p.y = -p.y;
            t = Camera.main.WorldToScreenPoint(t + p);

            GUI.Label(new Rect(t.x - 20, t.y - 10, 100, 20), titles[i - 1]);
        }
    }
}
