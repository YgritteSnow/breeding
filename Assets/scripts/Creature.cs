using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Creature : MonoBehaviour
{
    public const string PrefabDirectory = "Assets/resources/creature/";

    [SerializeField]
    public string Name = "";

    [SerializeField, Rename("基础生命值", 0, 20), Header("怪物属性补正")]
    public int Life = 0;

    [SerializeField, Rename("基础法力值", 0, 20)]
    public int Mana = 0;

    [SerializeField, Rename("基础物理攻击力", 0, 20)]
    public int PhysicalAttack = 0;

    [SerializeField, Rename("基础法术攻击力", 0, 20)]
    public int MagicalAttack = 0;

    [SerializeField, Rename("基础物理防御力", 0, 20)]
    public int PhysicalDefense = 0;

    [SerializeField, Rename("基础法术防御力", 0, 20)]
    public int MagicalDefense = 0;

    [SerializeField, Rename("基础速度", 0, 20)]
    public int Speed = 0;

    [SerializeField, Rename("暴击几率", 0, 100)]
    public int CriticalRate = 5;

    [SerializeField, Rename("暴击比例", 1, 1000)]
    public int CriticalDamage = 150;
}
