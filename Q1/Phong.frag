#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

// ī�޶�� ����
uniform vec3 viewPos;
uniform vec3 lightPos;

// ���� �Ӽ�
const vec3 ka = vec3(0.0, 1.0, 0.0);       // ambient
const vec3 kd = vec3(0.0, 0.5, 0.0);       // diffuse
const vec3 ks = vec3(0.5, 0.5, 0.5);       // specular
const float shininess = 32.0;

// ���� �Ӽ�
const float Ia = 0.2;
const vec3 lightColor = vec3(1.0); // white light

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

    // ���� ���� (Gamma correction)
    vec3 gammaCorrected = pow(linearColor, vec3(1.0 / 2.2));

    FragColor = vec4(gammaCorrected, 1.0);
}
