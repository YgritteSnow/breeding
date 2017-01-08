using UnityEngine;

[System.Serializable]
public class DNA : MonoBehaviour
{
    public const string PrefabDirectory = "Assets/resources/DNA/";

    [SerializeField]
    public string Name;

    [SerializeField, Rename("DNA强度", 1, 100)]
    public int Intensity = 50;

    [SerializeField, Header("每 10%DNA 提供的属性"), Rename("生命值加成", 0, 20)]
    public int Life = 10;

    [SerializeField, Rename("法力值加成", 0, 20)]
    public int Mana = 10;

    [SerializeField, Rename("物理攻击力加成", 0, 20)]
    public int PhysicalAttack = 5;

    [SerializeField, Rename("法术攻击力加成", 0, 20)]
    public int MagicalAttack = 5;

    [SerializeField, Rename("物理防御力加成", 0, 20)]
    public int PhysicalDefense = 5;

    [SerializeField, Rename("法术防御力加成", 0, 20)]
    public int MagicalDefense = 5;

    [SerializeField, Rename("速度加成", 0, 20)]
    public int Speed = 5;
}
