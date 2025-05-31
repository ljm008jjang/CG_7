#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

// ī�޶�� ���� ��ġ
uniform vec3 viewPos;
uniform vec3 lightPos;

// ���� �Ӽ� (�ܺο��� ����)
uniform vec3 ka;          // ambient reflectivity
uniform vec3 kd;          // diffuse reflectivity
uniform vec3 ks;          // specular reflectivity
uniform float shininess;
uniform float gamma;

// ���� �Ӽ� (�ܺο��� ����)
uniform float Ia;         // ambient light intensity
uniform vec3 lightColor;  // light color

void main()
{
    // ��� ����ȭ
    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPos - FragPos);
    vec3 V = normalize(viewPos - FragPos);
    vec3 R = reflect(-L, N);

    // Ambient
    vec3 ambient = Ia * ka;

    // Diffuse
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * kd * lightColor;

    // Specular
    float spec = pow(max(dot(R, V), 0.0), shininess);
    vec3 specular = ks * spec * lightColor;

    // ���� ��
    vec3 linearColor = ambient + diffuse + specular;

    // ���� ����
    vec3 gammaCorrected = pow(linearColor, vec3(1.0 / gamma));

    FragColor = vec4(gammaCorrected, 1.0);
}
