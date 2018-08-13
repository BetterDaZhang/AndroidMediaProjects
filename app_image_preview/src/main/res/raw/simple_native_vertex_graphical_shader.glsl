uniform mat4 u_ProjectionMatrix;
uniform mat4 u_LookAtMatrix;
uniform mat4 u_RotationMatrix;

attribute vec4 a_Position;
attribute vec4 a_Color;

varying vec4 v_Color;

void main()
{
    v_Color = a_Color;

    gl_Position = u_ProjectionMatrix * u_LookAtMatrix * u_RotationMatrix * a_Position;

//    gl_Position = a_Position;

//gl_Position = u_ProjectionMatrix * u_LookAtMatrix * a_Position;

//    gl_Position = u_ProjectionMatrix  * a_Position;
    gl_PointSize = 10.0;
}