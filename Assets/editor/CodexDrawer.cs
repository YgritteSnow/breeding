using UnityEngine;
using UnityEditor;

[CustomPropertyDrawer(typeof(CodexEntry))]
public class CodexDrawer : PropertyDrawer
{
    public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
    {
        return 40;
    }

    string GetString(SerializedProperty property, string varName)
    {
        Object var = property.FindPropertyRelative(varName).objectReferenceValue;
        return (var == null) ? "null" : var.name;
    }

    public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
    {
        EditorGUI.BeginProperty(position, label, property);
        Rect elementPos = position;

        elementPos.y += 4;
        elementPos.height = 16;

        EditorGUI.BeginChangeCheck();
        EditorGUI.PropertyField(elementPos, property.FindPropertyRelative("Creature"));
        if (EditorGUI.EndChangeCheck())
        {
            property.serializedObject.ApplyModifiedProperties();
        }

        elementPos.y += 16;
        elementPos.width = position.width / 2;
        EditorGUI.LabelField(elementPos, "[父]:" + GetString(property, "FatherDNA"));
        elementPos.x += elementPos.width;


        EditorGUI.LabelField(elementPos, "[母]:" + GetString(property, "MotherDNA"));
        elementPos.x = position.x;
        elementPos.width = position.width;
        EditorGUI.EndProperty();
    }

}
